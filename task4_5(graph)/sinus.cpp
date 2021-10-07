#include <windows.h> // заголовочный файл, содержащий WINAPI
#include <cmath>

// Прототип функции обработки сообщений с пользовательским названием:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
TCHAR mainMessage[] = L"Какой то-текст!"; // строка с сообщением

// Управляющая функция:
int WINAPI WinMain(HINSTANCE hInst, // дескриптор экземпляра приложения
    HINSTANCE hPrevInst, // не используем
    LPSTR lpCmdLine, // не используем
    int nCmdShow) // режим отображения окошка
{
    TCHAR szClassName[] = L"Мой класс"; // строка с именем класса
    HWND hMainWnd; // создаём дескриптор будущего окошка
    MSG msg; // создём экземпляр структуры MSG для обработки сообщений
    WNDCLASSEX wc; // создаём экземпляр, для обращения к членам класса WNDCLASSEX
    wc.cbSize = sizeof(wc); // размер структуры (в байтах)
    wc.style = CS_HREDRAW | CS_VREDRAW; // стиль класса окошка
    wc.lpfnWndProc = WndProc; // указатель на пользовательскую функцию
    wc.lpszMenuName = NULL; // указатель на имя меню (у нас его нет)
    wc.lpszClassName = szClassName; // указатель на имя класса
    wc.cbWndExtra = NULL; // число освобождаемых байтов в конце структуры
    wc.cbClsExtra = NULL; // число освобождаемых байтов при создании экземпляра приложения
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO); // декриптор пиктограммы
    wc.hIconSm = LoadIcon(NULL, IDI_WINLOGO); // дескриптор маленькой пиктограммы (в трэе)
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); // дескриптор курсора
    wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH); // дескриптор кисти для закраски фона окна
    wc.hInstance = hInst; // указатель на строку, содержащую имя меню, применяемого для класса
    if (!RegisterClassEx(&wc)) {
        // в случае отсутствия регистрации класса:
        MessageBox(NULL, L"Не получилось зарегистрировать класс!", L"Ошибка", MB_OK);
        return NULL; // возвращаем, следовательно, выходим из WinMain
    }
    // Функция, создающая окошко:
    hMainWnd = CreateWindow(
        szClassName, // имя класса
        L"График синуса", // имя окошка (то что сверху)
        WS_OVERLAPPEDWINDOW | WS_VSCROLL, // режимы отображения окошка
        CW_USEDEFAULT, // позиция окошка по оси х
        NULL, // позиция окошка по оси у (раз дефолт в х, то писать не нужно)
        500, // ширина окошка
        500, // высота окошка (раз дефолт в ширине, то писать не нужно)
        (HWND)NULL, // дескриптор родительского окна
        NULL, // дескриптор меню
        HINSTANCE(hInst), // дескриптор экземпляра приложения
        NULL); // ничего не передаём из WndProc
    if (!hMainWnd) {
        // в случае некорректного создания окошка (неверные параметры и тп):
        MessageBox(NULL, L"Не получилось создать окно!", L"Ошибка", MB_OK);
        return NULL;
    }
    ShowWindow(hMainWnd, nCmdShow); // отображаем окошко
    UpdateWindow(hMainWnd); // обновляем окошко
    while (GetMessage(&msg, NULL, NULL, NULL)) { // извлекаем сообщения из очереди, посылаемые фу-циями, ОС
        TranslateMessage(&msg); // интерпретируем сообщения
        DispatchMessage(&msg); // передаём сообщения обратно ОС
    }
    return msg.wParam; // возвращаем код выхода из приложения
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    HDC hDC; // создаём дескриптор ориентации текста на экране
    PAINTSTRUCT ps; // структура, сод-щая информацию о клиентской области (размеры, цвет и тп)
    RECT rect; // стр-ра, определяющая размер клиентской области
    COLORREF colorText = RGB(255, 0, 0); // задаём цвет текста
    HPEN PenX = CreatePen(PS_SOLID, 2, RGB(255, 255, 255));
    HPEN PenY = CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
    switch (uMsg) {
    case WM_PAINT: // если нужно нарисовать, то:
        hDC = BeginPaint(hWnd, &ps); // инициализируем контекст устройства
        GetClientRect(hWnd, &rect); // получаем ширину и высоту области для рисования
        SetTextColor(hDC, colorText); // устанавливаем цвет контекстного устройства      
        SelectObject(hDC, PenX);

        MoveToEx(hDC, 250, 50, NULL);
        LineTo(hDC, 245, 55);
        MoveToEx(hDC, 250, 50, NULL);
        LineTo(hDC, 255, 55);

        MoveToEx(hDC, 450, 250, NULL);
        LineTo(hDC, 445, 245);
        MoveToEx(hDC, 450, 250, NULL);
        LineTo(hDC, 445, 255);

        MoveToEx(hDC, 50, 250, NULL);
        LineTo(hDC, 450, 250);

        MoveToEx(hDC, 250, 50, NULL);
        LineTo(hDC, 250, 450);

        SelectObject(hDC, PenY);
        for (float x = -10.0; x <= 10.0f; x += 0.01f)
        {
            MoveToEx(hDC, 20 * x + 250, -20 * sin(x) + 250, NULL);
            LineTo(hDC, 20 * x + 250, -20 * sin(x) + 250);
        }
        EndPaint(hWnd, &ps); // заканчиваем рисовать
        break;
    case WM_DESTROY: // если окошко закрылось, то:
        PostQuitMessage(NULL); // отправляем WinMain() сообщение WM_QUIT
        break;
    default:
        return DefWindowProc(hWnd, uMsg, wParam, lParam); // если закрыли окошко
    }
    return NULL; // возвращаем значение
}

