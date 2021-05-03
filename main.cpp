#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include<SDL_ttf.h>
#include<SDL_mixer.h>

static Mix_Chunk* sound1 ;


using namespace std;
//phác hoạ tấm ảnh
SDL_Texture* loadTexture(const std::string& file, SDL_Renderer* ren);

//TTF_Font* TTF_OpenFont(const char *file, int ptsize);
//TTF_Font* font = NULL;
//font = TTF_OpenFont("Xerox Serif Wide.ttf", 20);

//vẽ tấm ảnh lên cửa sổ
void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y, int w, int h);
int step = 20;
SDL_Window* window;
SDL_Renderer* renderer;
SDL_Rect filled_rect;
int speed = 100;
int move_[4][2] =
{
    {0,1}, //Down
    {1,0}, //Right
    {-1,0}, //Left
    {0,-1} //Up
};

const int SCREEN_HEIGHT = 600;
const int SCREEN_WIDTH = 800;
const string WINDOW_TITLE = "Snake";
void logSDLError(std::ostream& os, const std::string& msg, bool fatal = false);
void initSDL(SDL_Window*& window, SDL_Renderer*& renderer);
void quitSDL(SDL_Window* window, SDL_Renderer* renderer);
void waitUntilKeyPressed();


class SnakeClass
{
public:
    SnakeClass();
    ~SnakeClass();
    //tạo food random
    void CreatApple();
    //xử lý
    void movement(int dir);
    bool GameOver();
    int MenuStartGame();
    int length;

private:
    SDL_Texture* H;
    SDL_Texture* GOV;
    SDL_Texture* BG;
    SDL_Texture* ExitButton;
    SDL_Texture* StartButton;
    vector <int> x, y, dir_;
    int apple_x, apple_y;
};

//định nghĩa hàm snakeclass trong class
SnakeClass::SnakeClass()
{
    srand(time(NULL));
    initSDL(window, renderer);

    H = loadTexture("Assets/BACKGR.png",renderer);

    GOV = loadTexture("Assets/Header.png",renderer);
    BG = loadTexture("Assets/menu.png", renderer);
    StartButton = loadTexture("Assets/Start_BTN.png", renderer);
    ExitButton = loadTexture("Assets/Exit_BTN.png", renderer);

    CreatApple();
    length = 3;
    for (int i = 0; i < length;i++)
    {
        //push_back thêm vào cuối vector
        x.push_back(SCREEN_WIDTH / 2);
        y.push_back(SCREEN_HEIGHT / 2);


        dir_.push_back(1);
    }
}


int SnakeClass::MenuStartGame()
{
    renderTexture(BG, renderer,0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
    renderTexture(StartButton,renderer,SCREEN_WIDTH/2-30,SCREEN_HEIGHT/4,SCREEN_WIDTH/10,SCREEN_HEIGHT/10);
    renderTexture(ExitButton,renderer,SCREEN_WIDTH/2-30,SCREEN_HEIGHT/2,SCREEN_WIDTH/10,SCREEN_HEIGHT/10);
    SDL_RenderPresent(renderer);
    SDL_Event e;
    while(true)
     while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            return 0;
        }
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if( 370 <= e.button.x && e.button.x <=450 )
            {
                if( 150 <= e.button.y && e.button.y <= 200 )
                    return 1;
                else if ( 300 <= e.button.y && e.button.y <= 350 )
                    return 0;
            }
        }
    }
}

void SnakeClass::CreatApple()
{
    apple_x = rand() * step % SCREEN_WIDTH;
    apple_y = rand() * step % SCREEN_HEIGHT;
    for (int i = 0;i < x.size();i++){
        if (apple_x == x[i] && apple_y == y[i])
            CreatApple();
    return;
    }
}

bool SnakeClass::GameOver()
{
    for(int i=0;i<x.size()-2;i++)
    {
        if(x.back() == x[i] && y.back() == y[i] )
        {
        cout<<"score:"<<length*10<<endl;
        renderTexture(GOV,renderer,SCREEN_WIDTH/2-SCREEN_WIDTH/10,SCREEN_HEIGHT/2-SCREEN_HEIGHT/10,SCREEN_WIDTH/5,SCREEN_HEIGHT/5);
        SDL_RenderPresent(renderer);
        SDL_Delay(2000);
        return 1;
        }
    }
    return 0;
}

void SnakeClass::movement(int dir)//xử lý
{
    x.push_back((x.back() + SCREEN_WIDTH + move_[dir][0] * step) % SCREEN_WIDTH);
    y.push_back((y.back() + SCREEN_HEIGHT + move_[dir][1] * step) % SCREEN_HEIGHT);
    dir_.push_back(dir);
    x.erase(x.begin(), x.begin() + 1);
    y.erase(y.begin(), y.begin() + 1);
            dir_.erase(dir_.begin(), dir_.begin() + 1);
    if (apple_x == x.back() && apple_y == y.back())
    {
        length++;
        if(speed>=30)
            speed -=2;
        x.push_back(apple_x);
        y.push_back(apple_y);
        dir_.push_back(dir);
        CreatApple();
        Mix_PlayChannel(-1,sound1,0) ;
    }
    SDL_Rect dstRect;
    dstRect.w = step;
    dstRect.h = step;
    dstRect.x = apple_x;
    dstRect.y = apple_y;
//    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);// blue
    //xóa tất cả render
    SDL_RenderClear(renderer);
    renderTexture(H,renderer,0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    //vẽ hình chữ nhật và tô màu cho quả táo
    SDL_RenderFillRect(renderer, &dstRect);
    for (int i = 0;i < length-1;i++)
    {
    dstRect.x = x[i];
    dstRect.y = y[i];

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &dstRect);
    }
    dstRect.x = x.back();
    dstRect.y = y.back();
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &dstRect);

    SDL_RenderPresent(renderer);
}


SnakeClass::~SnakeClass()
{
    speed = 100;
    x.clear();
    y.clear();
    dir_.clear();
    length = 3;
    for (int i = 0; i < length;i++)
    {
        //push_back thêm vào cuối vector
        x.push_back(SCREEN_WIDTH / 2);
        y.push_back(SCREEN_HEIGHT / 2);


        dir_.push_back(1);
    }
}

SnakeClass Snake;

int main(int argc, char* argv[])
{

    while( Snake.MenuStartGame() == 1)
    {
        waitUntilKeyPressed();
        Snake.~SnakeClass();
    }
    quitSDL(window, renderer);
    return 0;
}

SDL_Texture* loadTexture(const std::string& file, SDL_Renderer* ren)
{
    //Khởi tạo là nullptr để tránh lỗi 'dangling pointer'
    SDL_Texture* texture = nullptr;
    //Nạp ảnh từ tên file (với đường dẫn)
    SDL_Surface* loadedImage = IMG_Load(file.c_str());
    //Nếu không có lỗi, chuyển đổi về dạng texture and và trả về
    if (loadedImage != nullptr) {
        texture = SDL_CreateTextureFromSurface(ren, loadedImage);
        SDL_FreeSurface(loadedImage);
        //Đảm bảo việc chuyển đổi không có lỗi
        if (texture == nullptr) {
            logSDLError(std::cout, "CreateTextureFromSurface");
        }
    }
    else {
        logSDLError(std::cout, "LoadBMP");
    }
    return texture;
}

void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y, int w, int h)
{
    //Thiết lập hình chữ nhật đích mà chúng ta muốn vẽ ảnh vào trong
    SDL_Rect dst;
    dst.x = x;
    dst.y = y;
    dst.w = w;
    dst.h = h;
    //Đưa toàn bộ ảnh trong texture vào hình chữ nhật đích
    //(ảnh sẽ co dãn cho khớp với kích cỡ mới)
    SDL_RenderCopy(ren, tex, NULL, &dst);
}

//*****************************************************
// Các hàm chung về khởi tạo và huỷ SDL
void logSDLError(std::ostream& os, const std::string& msg, bool fatal)
{
    os << msg << " Error: " << SDL_GetError() << std::endl;
    if (fatal) {
        SDL_Quit();
        exit(1);
    }
}

void initSDL(SDL_Window*& window, SDL_Renderer*& renderer)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
        logSDLError(std::cout, "SDL_Init", true);

    if(Mix_OpenAudio(22050,MIX_DEFAULT_FORMAT,2,4096) == -1)
    {
        cout << "Not Audio" ;
    }


    sound1 = Mix_LoadWAV("Sound/Sound_Gun.wav") ;

//
//    if(sound1 == NULL )
//    {
//        cout << "Not soud1" ;
//    }

    window = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == nullptr) logSDLError(std::cout, "CreateWindow", true);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
        SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) logSDLError(std::cout, "CreateRenderer", true);

    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
}

void quitSDL(SDL_Window* window, SDL_Renderer* renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void waitUntilKeyPressed()
{
    int dir = 0;
    int direction = 0;
    SDL_Event e;
    bool is_game = true;
    int dem=0;
    while (is_game)
    {
        SDL_Delay(speed);
        dem++;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                is_game = false;
                break;
            }
            if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    is_game = false;
                    break;
                }
                if (e.key.keysym.sym == SDLK_LEFT||e.key.keysym.sym == SDLK_a) dir = 2;
                if (e.key.keysym.sym == SDLK_RIGHT||e.key.keysym.sym == SDLK_d) dir = 1;
                if (e.key.keysym.sym == SDLK_DOWN||e.key.keysym.sym == SDLK_s) dir = 0;
                if (e.key.keysym.sym == SDLK_UP||e.key.keysym.sym == SDLK_w) dir = 3;
                if ((dir + direction) % 3 == 0)
                    dir = direction;
            }
            direction = dir;
        }
        if( dem>Snake.length )
                if( Snake.GameOver() )return;
        Snake.movement(direction);
    }

}
