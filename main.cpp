#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "tiny_obj_loader.h"
#include "Mesh.h"

Mesh loadModel(const std::string& path) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;

    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str())) {
        std::cerr << "Erreur lors du chargement de l'OBJ : " << warn << err << std::endl;
        exit(1);
    }

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            Vertex vertex;

            // Position
            vertex.Position = glm::vec3(
                attrib.vertices[3 * index.vertex_index + 0],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 2]
            );

            // Normales
            if (!attrib.normals.empty()) {
                vertex.Normal = glm::vec3(
                    attrib.normals[3 * index.normal_index + 0],
                    attrib.normals[3 * index.normal_index + 1],
                    attrib.normals[3 * index.normal_index + 2]
                );
            }

            // Coordonnées de texture
            if (!attrib.texcoords.empty()) {
                vertex.TexCoords = glm::vec2(
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    attrib.texcoords[2 * index.texcoord_index + 1]
                );
            }

            vertices.push_back(vertex);
            indices.push_back(indices.size());
        }
    }

    return Mesh(vertices, indices);
}

// Callback pour ajuster la taille de la fenêtre
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Initialisation GLFW et GLAD
GLFWwindow* initializeWindow(int width, int height, const char* title) {
    if (!glfwInit()) {
        std::cerr << "Échec de l'initialisation de GLFW !" << std::endl;
        return nullptr;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (!window) {
        std::cerr << "Échec de la création de la fenêtre GLFW !" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Échec de l'initialisation de GLAD !" << std::endl;
        return nullptr;
    }

    glEnable(GL_DEPTH_TEST); // Activer le test de profondeur
    return window;
}

int main() {
    // Initialisation
    GLFWwindow* window = initializeWindow(800, 600, "Projet OpenGL");
    if (!window) return -1;

    // Boucle principale
    while (!glfwWindowShouldClose(window)) {
        // Effacer l'écran
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Logique de dessin ici...

        // Gestion des événements et affichage
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
