#include <GL/glut.h>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>

#define GL_SILENCE_DEPRECATION

std::string input = "";
std::string display = "";
float result = 0.0;
char lastOperation = 0;
bool showCalculator = false;

struct Button {
    float xmin, xmax, ymin, ymax;
    std::string label;
};

Button buttons[4][4] = {
    { {-0.8f, -0.6f, 0.6f, 0.8f, "1"}, {-0.6f, -0.4f, 0.6f, 0.8f, "2"}, {-0.4f, -0.2f, 0.6f, 0.8f, "3"}, {-0.2f, 0.0f, 0.6f, 0.8f, "+"} },
    { {-0.8f, -0.6f, 0.4f, 0.6f, "4"}, {-0.6f, -0.4f, 0.4f, 0.6f, "5"}, {-0.4f, -0.2f, 0.4f, 0.6f, "6"}, {-0.2f, 0.0f, 0.4f, 0.6f, "-"} },
    { {-0.8f, -0.6f, 0.2f, 0.4f, "7"}, {-0.6f, -0.4f, 0.2f, 0.4f, "8"}, {-0.4f, -0.2f, 0.2f, 0.4f, "9"}, {-0.2f, 0.0f, 0.2f, 0.4f, "*"} },
    { {-0.8f, -0.6f, 0.0f, 0.2f, "B"}, {-0.6f, -0.4f, 0.0f, 0.2f, "0"}, {-0.4f, -0.2f, 0.0f, 0.2f, "="}, {-0.2f, 0.0f, 0.0f, 0.2f, "C"} }
};

void displayText(float x, float y, std::string text) {
    glRasterPos2f(x, y);
    for (char c : text) {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
    }
}

void drawButton(Button button) {
    if (button.label == "=") {
        glColor3f(0.0, 0.0, 1.0); // Blue color for "=" button
    } else {
        glColor3f(1.0, 0.0, 0.0); // Red color for other buttons
    }
    glBegin(GL_QUADS);
    glVertex2f(button.xmin, button.ymin);
    glVertex2f(button.xmax, button.ymin);
    glVertex2f(button.xmax, button.ymax);
    glVertex2f(button.xmin, button.ymax);
    glEnd();

    glColor3f(1.0, 1.0, 1.0);
    displayText((button.xmin + button.xmax) / 2 - 0.05f, (button.ymin + button.ymax) / 2 - 0.05f, button.label);
    glColor3f(1.0, 0.0, 0.0);
}

void renderCalculator() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3f(1.0, 0.0, 0.0);

    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            drawButton(buttons[i][j]);
        }
    }

    displayText(-0.8f, 0.8f, display);
    glutSwapBuffers();
}

void renderIntro() {
    glClear(GL_COLOR_BUFFER_BIT);
    
    glColor3f(1.0, 1.0, 1.0);
    displayText(-0.2f, 0.1f, "Sakshi Sudarshan");
    displayText(-0.3f, -0.1f, "1RN21CS133");

    glutSwapBuffers();
}

std::string formatNumber(float num) {
    std::ostringstream out;
    out << std::fixed << std::setprecision(0) << num;
    return out.str();
}

void processInput() {
    float currentInput = std::stof(input);
    switch (lastOperation) {
        case '+':
            result += currentInput;
            break;
        case '-':
            result -= currentInput;
            break;
        case '*':
            result *= currentInput;
            break;
        case '/':
            if (currentInput != 0.0) {
                result /= currentInput;
            } else {
                display = "Error";
                input = "";
                return;
            }
            break;
        default:
            result = currentInput;
            break;
    }

    display =  lastOperation + formatNumber(currentInput);
    input = "";
}

void handleButtonPress(char button) {
    if (button >= '0' && button <= '9') {
        input += button;
        display += button;
    } else if (button == '+' || button == '-' || button == '*' || button == '/') {
        if (!input.empty()) {
            processInput();
        }
        lastOperation = button;
        display += button;
    } else if (button == '=') {
        if (!input.empty()) {
            processInput();
        }
        display = input + "=" + formatNumber(result);  
        lastOperation = 0;
        input = std::to_string(result);
    } else if (button == 'B') {
        if (!input.empty()) {
            input.pop_back();
            display.pop_back();
        }
    } else if (button == 'C') {
        input = "";
        display = "";
        result = 0.0;
        lastOperation = 0;
    }
}

void mouseClick(int button, int state, int x, int y) {
    if (showCalculator && state == GLUT_DOWN) {
        float fx = (x / 250.0f) - 1.0f;
        float fy = 1.0f - (y / 250.0f);

        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                Button b = buttons[i][j];
                if (fx >= b.xmin && fx <= b.xmax && fy >= b.ymin && fy <= b.ymax) {
                    handleButtonPress(b.label[0]);
                }
            }
        }
    }

    glutPostRedisplay();
}

void switchToCalculator(int value) {
    showCalculator = true;
    glutPostRedisplay();
}

void render() {
    if (showCalculator) {
        renderCalculator();
    } else {
        renderIntro();
    }
}

void myinit() {
    glClearColor(0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("OpenGL Calculator");
    myinit();
    glutDisplayFunc(render);
    glutMouseFunc(mouseClick);
    glutTimerFunc(5000, switchToCalculator, 0); 
    glutMainLoop();
    return 0;
}

