#include <SDL2/SDL.h>
#include <array>
#include <string>
#include <cmath>
#include <cstdlib>
#include <iostream>

std::array<unsigned char, 3> hsv2rgb(float h, float s, float l) {
  std::array<unsigned char,3> result;
  std::array<float,3> resultf;
  float c = (1. - abs(2.*l - 1.)) * s;
  int hprime = h / 60.;
  float x = c * (1.-abs(fmod(hprime,2)-1));
  switch(hprime) {
    case 0:
      resultf[0]=c;
      resultf[1]=x;
      resultf[2]=0;
      break;
    case 1:
      resultf[0]=x;
      resultf[1]=c;
      resultf[2]=0;
      break;
    case 2:
      resultf[0]=0;
      resultf[1]=c;
      resultf[2]=x;
      break;
    case 3:
      resultf[0]=0;
      resultf[1]=x;
      resultf[2]=c;
      break;
    case 4:
      resultf[0]=x;
      resultf[1]=0;
      resultf[2]=c;
      break;
    case 5:
      resultf[0]=c;
      resultf[1]=0;
      resultf[2]=x;
      break;
  }
  float m = l - c/2;
  result[0] = static_cast<unsigned char>(255*(resultf[0]+m));
  result[1] = static_cast<unsigned char>(255*(resultf[1]+m));
  result[2] = static_cast<unsigned char>(255*(resultf[2]+m));
  return result;
}

class WindowManager
{
  public:
    int window_width, window_height;
    SDL_Window* window;
    WindowManager();
    ~WindowManager();
};
WindowManager::WindowManager() : window_width(1920), window_height(1080),
  window(nullptr)
{
  SDL_Init(SDL_INIT_VIDEO);
  window = SDL_CreateWindow("OpenMC Explorer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      window_width, window_height, SDL_WINDOW_SHOWN);

  if (window == nullptr)
    std::cerr << "Failed to create window instance." << std::endl;
}
WindowManager::~WindowManager()
{
  SDL_DestroyWindow(window);
  SDL_Quit();
}

template<typename T>
void p(std::array<T,3> c)
{
  std::cout << "asdf:" << std::endl;
  std::cout << (int)c[0] << std::endl;
  std::cout << (int)c[1] << std::endl;
  std::cout << (int)c[2] << std::endl;
}

int main()
{
  WindowManager window;
  SDL_Renderer* renderer;
  SDL_Event event;
  float col = 0.0;
  int nsubloop = 2; // 20000;
  int ncolorstep = 1;
  int speed = 20;
  bool running = true;

  srand(0); // random seed
  unsigned char r=120, g=120, b=120;

  renderer = SDL_CreateRenderer(window.window, -1, SDL_RENDERER_ACCELERATED || SDL_HINT_RENDER_VSYNC);
  if (renderer == nullptr)
  {
    std::cerr<< "unable to make renderer instance:"
         << std::string(SDL_GetError()) << std::endl;
  }
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
  SDL_RenderClear(renderer);


  int posx, posy;
  posx = window.window_width/2;
  posy = window.window_height/2;

  while (running) {
    
    // Process keyboard input
    while (SDL_PollEvent(&event)) {
      switch (event.type) {
        case SDL_QUIT:
          running=false;
          break;
        case SDL_KEYDOWN:
          switch (event.key.keysym.sym) {
            case SDLK_UP:
              nsubloop += nsubloop | 1;
              break;
            case SDLK_DOWN:
              nsubloop -= nsubloop;
              if (nsubloop < 0) nsubloop = 1;
              break;
            case SDLK_LEFT:
              ncolorstep -= 1;
              if (ncolorstep < 0) ncolorstep=0;
              break;
            case SDLK_RIGHT:
              ncolorstep += 1;
              if (ncolorstep < 0) ncolorstep=0;
              break;
            case SDLK_w:
              posy -= speed;
              break;
            case SDLK_s:
              posy += speed;
              break;
            case SDLK_a:
              posx -= speed;
              break;
            case SDLK_d:
              posx += speed;
              break;
            case SDLK_c:
              SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
              SDL_RenderClear(renderer);
              break;
          }
          break;
      }
    }

    for (int kk=0; kk<nsubloop; ++kk) {
      // Sample direction to move
      int dir = rand() % 4;
      switch (dir) {
        case 0:
          posx += 1;
          break;
        case 1:
          posx -= 1;
          break;
        case 2:
          posy += 1;
          break;
        case 3:
          posy -= 1;
          break;
      }
      SDL_RenderDrawPoint(renderer, posx, posy);
    }

    if (posx < 0) posx=0;
    if (posy < 0) posy=0;
    if (posx > window.window_width) posx=window.window_width;
    if (posy > window.window_height) posy=window.window_height;

    // paint point
    int dr = rand()%3;
    int dg = rand()%3;
    int db = rand()%3;
    switch (dr) {
      case 0:
        r += ncolorstep;
        break;
      case 1:
        r -= ncolorstep;
        break;
    }
    switch (dg) {
      case 0:
        g += ncolorstep;
        break;
      case 1:
        g -= ncolorstep;
        break;
    }
    switch (db) {
      case 0:
        b += ncolorstep;
        break;
      case 1:
        b -= ncolorstep;
        break;
    }

    SDL_SetRenderDrawColor(renderer, r, g, b, 255);

    SDL_RenderPresent(renderer);
  }
  SDL_DestroyRenderer(renderer);
}
