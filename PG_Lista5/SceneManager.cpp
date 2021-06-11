#include "SceneManager.h"

static bool keys[1024];
static bool resized;
static GLuint width, height;

SceneManager::SceneManager() {}

SceneManager::~SceneManager() {}

void SceneManager::initialize(GLuint w, GLuint h) {
	width = w;
	height = h;
	
	initializeGraphics();
}

void SceneManager::initializeGraphics() {
	glfwInit();

	window = glfwCreateWindow(width, height, "Hello Sprites", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	glfwSetWindowSizeCallback(window, resize);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	addShader("../shaders/transformations.vs", "../shaders/transformations.frag");

	setupScene();

	resized = true;
}

void SceneManager::addShader(string vFilename, string fFilename) {
	shader = new Shader (vFilename.c_str(), fFilename.c_str());
}


void SceneManager::key_callback(GLFWwindow * window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}
}

void SceneManager::resize(GLFWwindow * window, int w, int h) {
	width = w;
	height = h;
	resized = true;

	glViewport(0, 0, width, height);
}

void SceneManager::update() {
	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void SceneManager::render() {
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (resized) {
		setupCamera2D();
		resized = false;
	}

	for (int i = 0; i < objects.size(); i++) {
		objects[i]->update();
		objects[i]->draw();
	}
}

void SceneManager::run() {
	while (!glfwWindowShouldClose(window)) {
		glfwPollEvents();

		update();

		render();
		
		glfwSwapBuffers(window);
	}
}

void SceneManager::finish() {
	glfwTerminate();
}


void SceneManager::setupScene()
{
	Sprite* obj = new Sprite;
	obj->setDimension(glm::vec3(640.0f, 374.0f, 1.0f));
	obj->setPosition(glm::vec3(400.0f, 280.0f, 0.0));
	obj->setShader(shader);
	objects.push_back(obj);

	unsigned int texID = loadTexture("../textures/legal.jpg");
	objects[0]->setTexture(texID);

	//Definindo a janela do mundo (ortho2D)
	ortho2D[0] = 0.0f;
	ortho2D[1] = 800.0f;
	ortho2D[2] = 0.0f;
	ortho2D[3] = 600.0f;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void SceneManager::setupCamera2D() {
	float zNear = -1.0, zFar = 1.0;

	projection = glm::ortho(ortho2D[0], ortho2D[1], ortho2D[2], ortho2D[3], zNear, zFar);

	GLint projLoc = glGetUniformLocation(shader->ID, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

unsigned int SceneManager::loadTexture(string filename) {
	unsigned int texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;
	
	unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
	
	if (data) {
		int tamanho = width * height * nrChannels;
		
		for (int i = 0; i < tamanho; i += nrChannels) {
			// EXERCICIO A
			// Canal vermelho
			//data[i + 1] = data[i];
			//data[i + 2] = data[i];
			
			// Canal verde
			//data[i] = data[i + 1];
			//data[i + 2] = data[i + 1];

			// Canal azul
			//data[i] = data[i + 2];
			//data[i + 1] = data[i + 2];
			// EXERCICIO A


			// EXERCICIO B
			//int media = (data[i] + data[i + 1] + data[i + 2]) / 3;
			//data[i] = media;
			//data[i + 1] = media;
			//data[i + 2] = media;
			// EXERCICIO B

			// EXERCICIO D
			//data[i] = data[i] ^ 255;
			//data[i + 1] = data[i + 1] ^ 255;
			//data[i + 2] = data[i + 2] ^ 255;
			// EXERCICIO D

			// EXERCICIO E
			//int media = (data[i] + data[i + 1] + data[i + 2]) / 3;
			
			//if (media < 200) {
			//	data[i] = 0;
			//	data[i + 1] = 0;
			//	data[i + 2] = 0;
			//}
			//else {
			//	data[i] = 255;
			//	data[i + 1] = 255;
			//	data[i + 2] = 255;
			//}
			// EXERCICIO E
		}


		if (nrChannels == 3) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);

	glActiveTexture(GL_TEXTURE0);

	return texture;
}
