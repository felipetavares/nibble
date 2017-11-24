#ifndef VIDEO_MEMORY_H
#define VIDEO_MEMORY_H

#include <cstdint>
#include <kernel/Memory.hpp>
#include <kernel/RenderBuffer.hpp>
#include <SFML/Graphics.hpp>
#include <gif_lib.h>

using namespace std;

class VideoMemory : public Memory {
    // Permite o acesso as fun��e protected
    friend class GPUCommandMemory;
    // Detalhes da mem�ria
	const uint64_t length;
	const uint64_t address;
	const unsigned int w, h;
    // Framebuffer da imagem final
    sf::RenderTexture framebuffer;
	// Textura contendo a imagem que � vis�vel na tela,
	// funciona como mem�ria de v�deo
	sf::RenderTexture gpuRenderTexture;
	// �reas para desenho do framebuffer e combinar os renders
    // em CPU e GPU
	sf::Sprite framebufferSpr, combineSpr;
    // Texturas para guardar os timings dos desenhos feitos pela cpu
    // e pela gpu, de forma que eles possam ser combinados na ordem
    // correta, facilitando para o desenvolvedor
    sf::RenderTexture gpuRenderTiming;
    sf::Texture cpuTiming;
    uint8_t *timingBuffer;
    // Vertex arrays utilizadas para desenhar informa��o de timing
    RenderBuffer gpuTimingBuffer, gpuQuadsBuffer;
    // Contador de draws
    uint32_t currentDraw;
    // Textura que permite a leitura e escrita.
    // Mem�ria de v�deo para opera��es n�o aceleradas em hardware
	sf::Texture cpuTexture;
    uint8_t *buffer;
	// Refer�ncia para a janela para que possamos desenhar para ela
	sf::RenderWindow &window;
    // C�digo e o shader utilizado para desenhar mixar texturas
    // e expandir para a tela
    const static string shaderVertex;
    const static string toRGBAShaderFragment;
    const static string combineShaderFragment;
    sf::Shader toRGBAShader, combineShader;
    // Textura utilizada como paleta pelo shader
    sf::Texture paletteTex;
    // Arquivo para salvar gifs
    GifFileType *gif;
    // Paleta do gif
    ColorMapObject *colormap;
public:
    const static uint64_t nibblesPerPixel;
    const static uint64_t bytesPerPixel;
    const static uint32_t vertexArrayLength;
public:
	VideoMemory(sf::RenderWindow&,
                const unsigned int,
                const unsigned int,
                const uint64_t);
	~VideoMemory();

    // Fecha arquivos abertos
    void close();

	void draw();

	uint64_t write(const uint64_t, const uint8_t*, const uint64_t);
	uint64_t read(const uint64_t, uint8_t*, const uint64_t);

	uint64_t size();
	uint64_t addr();

    // Chamado por paletteMemory quando o usu�rio troca a paleta
    void updatePalette(const uint8_t*);
protected:
    // Opera��es nas VertexArrays utilizadas para
    // desenho na GPU
    void drawGpuTiming(uint32_t,
                       uint32_t, uint32_t,
                       uint32_t, uint32_t);
    void drawGpuQuad(uint8_t,
                     uint16_t, uint16_t,
                     uint16_t, uint16_t);
    void execGpuCommand(uint8_t*);
private:
    void drawCpuTiming(uint32_t, uint64_t, uint64_t);
    void clearCpuTiming();
    // GIFs
    bool startCapturing(const string&);
    bool captureFrame();
    bool stopCapturing();
    ColorMapObject* getColorMap();
};

#endif /* VIDEO_MEMORY_H */
