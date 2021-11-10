#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(255);
	ofEnableDepthTest();

	this->noise_param = ofRandom(1000);

	auto ico_sphere = ofIcoSpherePrimitive(1, 5);
	this->base_location_list = ico_sphere.getMesh().getVertices();

	this->number_of_sphere = 30;
	while (this->sphere_list.size() < this->number_of_sphere) {

		int index = ofRandom(this->base_location_list.size());
		auto tmp_location = this->base_location_list[index];
		tmp_location = glm::normalize(tmp_location) * ofRandom(0, 110);

		auto radius = this->sphere_list.size() < 30 ? ofRandom(20, 50) : ofRandom(2, 20);

		bool flag = true;
		for (int i = 0; i < this->sphere_list.size(); i++) {

			if (glm::distance(tmp_location, get<1>(this->sphere_list[i])) < get<2>(this->sphere_list[i]) + radius) {

				flag = false;
				break;
			}
		}

		if (flag) {

			ofColor color;
			color.setHsb(ofRandom(255), 130, 230);

			auto size = (radius * 2) / sqrt(3);
			this->sphere_list.push_back(make_tuple(color, tmp_location, size));
		}
	}

	ico_sphere = ofIcoSpherePrimitive(300, 3);
	this->triangle_list.insert(this->triangle_list.end(), ico_sphere.getMesh().getUniqueFaces().begin(), ico_sphere.getMesh().getUniqueFaces().end());

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->mesh.clear();
	this->frame.clear();

	for (int i = 0; i < this->sphere_list.size(); i++) {

		auto color = get<0>(this->sphere_list[i]);
		auto location = get<1>(this->sphere_list[i]);
		auto radius = get<2>(this->sphere_list[i]) * 0.95;

		for (int k = 0; k < this->triangle_list.size(); k++) {

			glm::vec3 avg = (this->triangle_list[k].getVertex(0) + this->triangle_list[k].getVertex(1) + this->triangle_list[k].getVertex(2)) / 3;
			auto noise_value = ofNoise(glm::vec4(location * 0.1 + avg * 0.005, this->noise_param));

			if (noise_value < 0.5) {
			
				continue;
			}

			vector<glm::vec3> vertices;

			vertices.push_back(glm::normalize(this->triangle_list[k].getVertex(0)) * (radius + 2) - avg);
			vertices.push_back(glm::normalize(this->triangle_list[k].getVertex(1)) * (radius + 2) - avg);
			vertices.push_back(glm::normalize(this->triangle_list[k].getVertex(2)) * (radius + 2) - avg);

			vertices.push_back(glm::normalize(this->triangle_list[k].getVertex(0)) * (radius - 2) - avg);
			vertices.push_back(glm::normalize(this->triangle_list[k].getVertex(1)) * (radius - 2) - avg);
			vertices.push_back(glm::normalize(this->triangle_list[k].getVertex(2)) * (radius - 2) - avg);

			for (auto& vertex : vertices) {

				vertex += avg + location;
			}

			this->mesh.addVertices(vertices);
			this->frame.addVertices(vertices);

			this->mesh.addColor(ofColor(255));
			this->mesh.addColor(ofColor(255));
			this->mesh.addColor(ofColor(255));

			this->mesh.addColor(color);
			this->mesh.addColor(color);
			this->mesh.addColor(color);

			this->frame.addColor(ofColor(0));
			this->frame.addColor(ofColor(0));
			this->frame.addColor(ofColor(0));

			this->frame.addColor(ofColor(255));
			this->frame.addColor(ofColor(255));
			this->frame.addColor(ofColor(255));

			this->mesh.addTriangle(this->mesh.getNumVertices() - 1, this->mesh.getNumVertices() - 2, this->mesh.getNumVertices() - 3);
			this->mesh.addTriangle(this->mesh.getNumVertices() - 4, this->mesh.getNumVertices() - 5, this->mesh.getNumVertices() - 6);

			this->mesh.addTriangle(this->mesh.getNumVertices() - 1, this->mesh.getNumVertices() - 2, this->mesh.getNumVertices() - 5);
			this->mesh.addTriangle(this->mesh.getNumVertices() - 1, this->mesh.getNumVertices() - 5, this->mesh.getNumVertices() - 4);

			this->mesh.addTriangle(this->mesh.getNumVertices() - 1, this->mesh.getNumVertices() - 3, this->mesh.getNumVertices() - 6);
			this->mesh.addTriangle(this->mesh.getNumVertices() - 1, this->mesh.getNumVertices() - 6, this->mesh.getNumVertices() - 4);

			this->mesh.addTriangle(this->mesh.getNumVertices() - 2, this->mesh.getNumVertices() - 3, this->mesh.getNumVertices() - 6);
			this->mesh.addTriangle(this->mesh.getNumVertices() - 2, this->mesh.getNumVertices() - 6, this->mesh.getNumVertices() - 5);

			this->frame.addIndex(this->frame.getNumVertices() - 1); this->frame.addIndex(this->frame.getNumVertices() - 2);
			this->frame.addIndex(this->frame.getNumVertices() - 2); this->frame.addIndex(this->frame.getNumVertices() - 3);
			this->frame.addIndex(this->frame.getNumVertices() - 1); this->frame.addIndex(this->frame.getNumVertices() - 3);

			this->frame.addIndex(this->frame.getNumVertices() - 4); this->frame.addIndex(this->frame.getNumVertices() - 5);
			this->frame.addIndex(this->frame.getNumVertices() - 5); this->frame.addIndex(this->frame.getNumVertices() - 6);
			this->frame.addIndex(this->frame.getNumVertices() - 4); this->frame.addIndex(this->frame.getNumVertices() - 6);

			this->frame.addIndex(this->frame.getNumVertices() - 1); this->frame.addIndex(this->frame.getNumVertices() - 4);
			this->frame.addIndex(this->frame.getNumVertices() - 2); this->frame.addIndex(this->frame.getNumVertices() - 5);
			this->frame.addIndex(this->frame.getNumVertices() - 3); this->frame.addIndex(this->frame.getNumVertices() - 6);
		}
	}

	if (ofGetFrameNum() % 60 < 30) {

		this->noise_param += ofMap(ofGetFrameNum() % 60, 0, 30, 0.3, 0.001);
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 0.66666666666666666);

	this->mesh.drawFaces();
	this->frame.drawWireframe();

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}