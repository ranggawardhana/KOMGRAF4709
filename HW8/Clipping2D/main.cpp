#include <iostream>
#include <cmath>

class Point {
public:
    float x, y;

    Point(float x, float y) : x(x), y(y) {}
};

class Line {
public:
    Point start, end;

    Line(Point start, Point end) : start(start), end(end) {}
};

class Clipping {
public:
    Point minWindow, maxWindow;
    Point minViewport, maxViewport;

    Clipping(Point minWindow, Point maxWindow, Point minViewport, Point maxViewport)
        : minWindow(minWindow), maxWindow(maxWindow), minViewport(minViewport), maxViewport(maxViewport) {}

    // Cohen-Sutherland Clipping Algorithm
    void clipLine(Line& line) {
        int codeStart = computeCode(line.start);
        int codeEnd = computeCode(line.end);
        bool accept = false;

        while (true) {
            if (!(codeStart | codeEnd)) {
                accept = true;
                break;
            } else if (codeStart & codeEnd) {
                break;
            } else {
                float x, y;
                int codeOutside = (codeStart != 0) ? codeStart : codeEnd;

                if (codeOutside & 8) {
                    x = line.start.x + (line.end.x - line.start.x) * (maxViewport.y - line.start.y) / (line.end.y - line.start.y);
                    y = maxViewport.y;
                } else if (codeOutside & 4) {
                    x = line.start.x + (line.end.x - line.start.x) * (minViewport.y - line.start.y) / (line.end.y - line.start.y);
                    y = minViewport.y;
                } else if (codeOutside & 2) {
                    y = line.start.y + (line.end.y - line.start.y) * (maxViewport.x - line.start.x) / (line.end.x - line.start.x);
                    x = maxViewport.x;
                } else if (codeOutside & 1) {
                    y = line.start.y + (line.end.y - line.start.y) * (minViewport.x - line.start.x) / (line.end.x - line.start.x);
                    x = minViewport.x;
                }

                if (codeOutside == codeStart) {
                    line.start.x = x;
                    line.start.y = y;
                    codeStart = computeCode(line.start);
                } else {
                    line.end.x = x;
                    line.end.y = y;
                    codeEnd = computeCode(line.end);
                }
            }
        }

        if (accept) {
            std::cout << "Line after clipping: (" << line.start.x << ", " << line.start.y << ") to ("
                      << line.end.x << ", " << line.end.y << ")\n";
        } else {
            std::cout << "Line is completely outside the viewport and rejected.\n";
        }
    }

    int computeCode(const Point& p) {
        int code = 0;

        if (p.x < minViewport.x)
            code |= 1; // to the left of viewport
        else if (p.x > maxViewport.x)
            code |= 2; // to the right of viewport

        if (p.y < minViewport.y)
            code |= 4; // below the viewport
        else if (p.y > maxViewport.y)
            code |= 8; // above the viewport

        return code;
    }
};

int main() {
    Point minWindow(10, 10);
    Point maxWindow(50, 50);
    Point minViewport(20, 20);
    Point maxViewport(40, 40);

    Line inputLine(Point(5, 15), Point(60, 30));

    Clipping clipping(minWindow, maxWindow, minViewport, maxViewport);
    clipping.clipLine(inputLine);

    return 0;
}
