//
//  Game.cpp
//
//  Copyright � 2018 Alun Evans. All rights reserved.
//

#include "Game.h"
#include "Shader.h"
#include "extern.h"
#include "Parsers.h"

Game::Game() {

}

//Nothing here yet
void Game::init(int w, int h) {

	window_width_ = w; window_height_ = h;
	//******* INIT SYSTEMS *******

	//init systems except debug, which needs info about scene
	control_system_.init();
	graphics_system_.init(window_width_, window_height_, "data/assets/");
	debug_system_.init(&graphics_system_);
    script_system_.init(&control_system_);
	gui_system_.init(window_width_, window_height_);


	/******** SHADERS **********/

    graphics_system_.screen_background_color = lm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    
	Shader* phong_shader = graphics_system_.loadShader("data/shaders/phong.vert", "data/shaders/phong.frag");


 //   //create scene contents here
	//int ball_entity = ECS.createEntity("ball");
	//Mesh& ball_mesh = ECS.createComponentForEntity<Mesh>(ball_entity);
	//ball_mesh.geometry = graphics_system_.createGeometryFromFile("data/assets/ball.obj");
	//ball_mesh.material = graphics_system_.createMaterial();
	//graphics_system_.getMaterial(ball_mesh.material).shader_id = phong_shader->program;
	
	//Parsers::parseAnimation("data/assets/bounce.anim");
    
	//create scene contents here
	int plane_entity = ECS.createEntity("plane");
	Mesh& plane_mesh = ECS.createComponentForEntity<Mesh>(plane_entity);
	plane_mesh.geometry = graphics_system_.createGeometryFromFile("data/assets/plane_20x20.obj");
	plane_mesh.material = graphics_system_.createMaterial();
	graphics_system_.getMaterial(plane_mesh.material).shader_id = phong_shader->program;
	ECS.getComponentFromEntity<Transform>(plane_entity).translate(0.0,-1.0,0.0);

	//My tweening objects here:
	 //create scene contents here
	int tweening_cube_entity = ECS.createEntity("TweeningCube");
	Mesh& tweening_cube_mesh = ECS.createComponentForEntity<Mesh>(tweening_cube_entity);
	tweening_cube_mesh.geometry = graphics_system_.createGeometryFromFile("data/assets/cubemap.obj");
	graphics_system_.getMaterial(tweening_cube_mesh.material).shader_id = phong_shader->program;
	ECS.getComponentFromEntity<Transform>(tweening_cube_entity).translate(10.0, 10.0, 0.0);
	//-------------------------------------------------------------------------------------------------------
	//EaseOutCube
	int TweeningCube1_entity = ECS.createEntity("TweeningCube1");
	Mesh& tweeningCube1_mesh = ECS.createComponentForEntity<Mesh>(TweeningCube1_entity);
	tweeningCube1_mesh.geometry = graphics_system_.createGeometryFromFile("data/assets/cubemap.obj");
	graphics_system_.getMaterial(tweeningCube1_mesh.material).shader_id = phong_shader->program;
	ECS.getComponentFromEntity<Transform>(TweeningCube1_entity).translate(-10.0, 2.0, 0.0);

	//-------------------------------------------------------------------------------------------------------
	//EaseOutCube
	int TweeningCube2_entity = ECS.createEntity("TweeningCube2");
	Mesh& tweeningCube2_mesh = ECS.createComponentForEntity<Mesh>(TweeningCube2_entity);
	tweeningCube2_mesh.geometry = graphics_system_.createGeometryFromFile("data/assets/cubemap.obj");
	graphics_system_.getMaterial(tweeningCube2_mesh.material).shader_id = phong_shader->program;
	ECS.getComponentFromEntity<Transform>(TweeningCube2_entity).translate(10.0, 2.0, 0.0);
	//-------------------------------------------------------------------------------------------------------
	//Bounce Cube tween
	int TweeningCube3_entity = ECS.createEntity("TweeningCube3");
	Mesh& tweeningCube3_mesh = ECS.createComponentForEntity<Mesh>(TweeningCube3_entity);
	tweeningCube3_mesh.geometry = graphics_system_.createGeometryFromFile("data/assets/cubemap.obj");
	graphics_system_.getMaterial(tweeningCube3_mesh.material).shader_id = phong_shader->program;
	ECS.getComponentFromEntity<Transform>(TweeningCube3_entity).translate(10.0, 2.0, 0.0);

	//Lineal tweening
	tweening_system_.Tween(lm::vec3(5.0f, 00.0f, 5.0f), lm::vec3(5.0f, 10.0f, 5.0f), 24, 0, lm::vec3(0.0f, 0.0f, 0.0f), lm::vec3(0.0f, 5.0f, 0.0f), 0, "TweeningCube");
	//cubicInterpolation
	tweening_system_.Tween(lm::vec3(-5.0f, 0.0f, -5.0f), lm::vec3(-5.0f, 10.0f, -5.0f), 24, 1, lm::vec3(0.0f, 0.0f, 0.0f), lm::vec3(0.0f, 5.0f, 0.0f), 1, "TweeningCube1");
	//EasOutSine
	tweening_system_.Tween(lm::vec3(5.0f, 0.0f, -5.0f), lm::vec3(5.0f, 10.0f, -5.0f), 24, 2, lm::vec3(0.0f, 0.0f, 0.0f), lm::vec3(0.0f, 5.0f, 0.0f), 2, "TweeningCube2");
	//EaseOutBounce
	tweening_system_.Tween(lm::vec3(-5.0f,10.0f, 5.0f), lm::vec3(-5.0f, 0.0f, 5.0f), 24, 4, lm::vec3(0.0f, 0.0f, 0.0f), lm::vec3(0.0f, 5.0f, 0.0f), 2, "TweeningCube3");



    int light_ent = ECS.createEntity("Light");
    Light& light = ECS.createComponentForEntity<Light>(light_ent);
    light.direction = lm::vec3(-1.0f, -1.0f, -1.0f);
    light.type = LightTypeDirectional;

	//create camera
	createFreeCamera_();
    
    //******* LATE INIT AFTER LOADING RESOURCES *******//
    graphics_system_.lateInit();
    script_system_.lateInit();

    debug_system_.lateInit();

	debug_system_.setActive(true);

}

//update each system in turn
void Game::update(float dt) {

	if (ECS.getAllComponents<Camera>().size() == 0) {print("There is no camera set!"); return;}

	//update input
	control_system_.update(dt);

	//collision
	collision_system_.update(dt);
    
	//Animation system
	//animation_system_.update(dt);

	//Animation system
	tweening_system_.update(dt);

	//scripts
	script_system_.update(dt);

	//render
	graphics_system_.update(dt);
    
	//gui
	gui_system_.update(dt);

	//debug
	debug_system_.update(dt);
   
}
//update game viewports
void Game::update_viewports(int window_width, int window_height) {
	window_width_ = window_width;
	window_height_ = window_height;

	auto& cameras = ECS.getAllComponents<Camera>();
	for (auto& cam : cameras) {
		cam.setPerspective(60.0f*DEG2RAD, (float)window_width_ / (float) window_height_, 0.01f, 10000.0f);
	}

	graphics_system_.updateMainViewport(window_width_, window_height_);
}


int Game::createFreeCamera_() {
	int ent_player = ECS.createEntity("PlayerFree");
	Camera& player_cam = ECS.createComponentForEntity<Camera>(ent_player);
	lm::vec3 the_position(25.325, 17.425, -23.141);;
	ECS.getComponentFromEntity<Transform>(ent_player).translate(the_position);
	player_cam.position = the_position;
	player_cam.forward = lm::vec3(-0.722, -0.466, 0.510 );
	player_cam.setPerspective(60.0f*DEG2RAD, (float)window_width_/(float)window_height_, 0.1f, 10000.0f);

	ECS.main_camera = ECS.getComponentID<Camera>(ent_player);

	control_system_.control_type = ControlTypeFree;

	return ent_player;
}

int Game::createPlayer_(float aspect, ControlSystem& sys) {
	int ent_player = ECS.createEntity("PlayerFPS");
	Camera& player_cam = ECS.createComponentForEntity<Camera>(ent_player);
	lm::vec3 the_position(0.0f, 3.0f, 5.0f);
	ECS.getComponentFromEntity<Transform>(ent_player).translate(the_position);
	player_cam.position = the_position;
	player_cam.forward = lm::vec3(0.0f, 0.0f, -1.0f);
	player_cam.setPerspective(60.0f*DEG2RAD, aspect, 0.01f, 10000.0f);

	//FPS colliders 
	//each collider ray entity is parented to the playerFPS entity
	int ent_down_ray = ECS.createEntity("Down Ray");
	Transform& down_ray_trans = ECS.getComponentFromEntity<Transform>(ent_down_ray);
	down_ray_trans.parent = ECS.getComponentID<Transform>(ent_player); //set parent as player entity *transform*!
	Collider& down_ray_collider = ECS.createComponentForEntity<Collider>(ent_down_ray);
	down_ray_collider.collider_type = ColliderTypeRay;
	down_ray_collider.direction = lm::vec3(0.0, -1.0, 0.0);
	down_ray_collider.max_distance = 100.0f;

	int ent_left_ray = ECS.createEntity("Left Ray");
	Transform& left_ray_trans = ECS.getComponentFromEntity<Transform>(ent_left_ray);
	left_ray_trans.parent = ECS.getComponentID<Transform>(ent_player); //set parent as player entity *transform*!
	Collider& left_ray_collider = ECS.createComponentForEntity<Collider>(ent_left_ray);
	left_ray_collider.collider_type = ColliderTypeRay;
	left_ray_collider.direction = lm::vec3(-1.0, 0.0, 0.0);
	left_ray_collider.max_distance = 1.0f;

	int ent_right_ray = ECS.createEntity("Right Ray");
	Transform& right_ray_trans = ECS.getComponentFromEntity<Transform>(ent_right_ray);
	right_ray_trans.parent = ECS.getComponentID<Transform>(ent_player); //set parent as player entity *transform*!
	Collider& right_ray_collider = ECS.createComponentForEntity<Collider>(ent_right_ray);
	right_ray_collider.collider_type = ColliderTypeRay;
	right_ray_collider.direction = lm::vec3(1.0, 0.0, 0.0);
	right_ray_collider.max_distance = 1.0f;

	int ent_forward_ray = ECS.createEntity("Forward Ray");
	Transform& forward_ray_trans = ECS.getComponentFromEntity<Transform>(ent_forward_ray);
	forward_ray_trans.parent = ECS.getComponentID<Transform>(ent_player); //set parent as player entity *transform*!
	Collider& forward_ray_collider = ECS.createComponentForEntity<Collider>(ent_forward_ray);
	forward_ray_collider.collider_type = ColliderTypeRay;
	forward_ray_collider.direction = lm::vec3(0.0, 0.0, -1.0);
	forward_ray_collider.max_distance = 1.0f;

	int ent_back_ray = ECS.createEntity("Back Ray");
	Transform& back_ray_trans = ECS.getComponentFromEntity<Transform>(ent_back_ray);
	back_ray_trans.parent = ECS.getComponentID<Transform>(ent_player); //set parent as player entity *transform*!
	Collider& back_ray_collider = ECS.createComponentForEntity<Collider>(ent_back_ray);
	back_ray_collider.collider_type = ColliderTypeRay;
	back_ray_collider.direction = lm::vec3(0.0, 0.0, 1.0);
	back_ray_collider.max_distance = 1.0f;

	//the control system stores the FPS colliders 
	sys.FPS_collider_down = ECS.getComponentID<Collider>(ent_down_ray);
	sys.FPS_collider_left = ECS.getComponentID<Collider>(ent_left_ray);
	sys.FPS_collider_right = ECS.getComponentID<Collider>(ent_right_ray);
	sys.FPS_collider_forward = ECS.getComponentID<Collider>(ent_forward_ray);
	sys.FPS_collider_back = ECS.getComponentID<Collider>(ent_back_ray);

	ECS.main_camera = ECS.getComponentID<Camera>(ent_player);

	sys.control_type = ControlTypeFPS;

	return ent_player;
}

