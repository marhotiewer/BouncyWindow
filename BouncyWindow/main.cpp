#define UNICODE

#include <windows.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <vector>

using namespace std;
using namespace std::chrono;

INT bounces = -1;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        HFONT hFont, hTmp;
        hFont = CreateFont(80, 0, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 2, 0, L"SYSTEM_FIXED_FONT");
        hTmp = (HFONT)SelectObject(hdc, hFont);
        
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

        wchar_t counter_str[5] = { };
        wsprintf(counter_str, L"%d", ++bounces);
        TextOutW(hdc, 5, 0, counter_str, 5);

        EndPaint(hwnd, &ps);
    }
    return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void game(HWND hwnd)
{
    RECT desktop = {};
    RECT window = {};
    INT velocity_x = 1;
    INT velocity_y = 1;

    time_point<system_clock> t = system_clock::now();
    const HWND hdsktp = GetDesktopWindow();
    GetWindowRect(hdsktp, &desktop);

    while (true)
    {
        GetWindowRect(hwnd, &window);
        if (window.bottom > desktop.bottom)
        {
            velocity_y = -1;
            RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
        }
        if (window.right > desktop.right)
        {
            velocity_x = -1;
            RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
        }
        if (window.top < desktop.top)
        {
            velocity_y = 1;
            RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
        }
        if (window.left < desktop.left)
        {
            velocity_x = 1;
            RedrawWindow(hwnd, NULL, NULL, RDW_INVALIDATE);
        }
        SetWindowPos(hwnd, HWND_TOPMOST, window.left + velocity_x, window.top + velocity_y, 0, 0, SWP_NOSIZE | SWP_NOREDRAW);
        t += milliseconds(5);
        this_thread::sleep_until(t);
    }
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        L"Learn to Program Windows",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style
        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 165, 80,
        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );

    SetWindowLong(hwnd, GWL_STYLE, 0);

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    std::thread thread_object(game, hwnd);

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
