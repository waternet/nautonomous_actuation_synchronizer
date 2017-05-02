/*
 * SendActuation.cpp
 *
 *  Created on: Apr 16, 2016
 *      Author: zeeuwe01
 */

#include "../include/nautonomous_actuation_synchronizer/send_actuation.hpp"

/**
 *\brief  Initialized the serial connection, set the used port and bautrate.
 *\params
 *\return
 */
bool actuation_init_serial() {
	ROS_INFO("Opening Sabertooth drive, testing %d", testing_sabertooth);
	testing_sabertooth = false;

	if(!testing_sabertooth){
		ROS_INFO("Opening Sabertooth driver");
		//Open serial port sabertooth at 9600 baud with 250 ms timeout.
		actuation_serial = new serial::Serial(string("/dev/nautonomous/actuation"),115200, serial::Timeout::simpleTimeout(250));
		ROS_INFO("Serial open: %d", actuation_serial->isOpen());

		//   (Deprecated, done by actuation platform)
		//ros::Duration(1).sleep();
		
		//Prepare the serial timeout for both motor drivers and send them.
		//uint8_t serial_timeout_propulsion[4], serial_timeout_conveyor[4];
		//sabertooth_advanced_serial_timeout(&serial_timeout_propulsion[0], &serial_timeout_conveyor[0]);

		//Write timeout to propulsion motor driver
		//int bytes = actuation_serial->write(&serial_timeout_propulsion[0], 4);

		//ros::Duration(0.01).sleep();

		//Write timeout to conveyor belt motor driver
		//bytes += actuation_serial->write(&serial_timeout_conveyor[0], 4);
		return 1;
	} else {
		return 0;
	}
}



/**
 *\brief  Deinitialize and removes the serial connection.
 *\params
 *\return
 */
void actuation_deinit_serial() {
	if(actuation_serial){
		actuation_serial->close();
		delete actuation_serial;
		actuation_serial = nullptr;
	}
}

/**
 *\brief  Deprecated, Send debug twist.
 *\params const geometry_msgs::Twist propulsion
 *\return
 */
void actuation_send_debug_twist(const geometry_msgs::Twist::ConstPtr& propulsion) {

}

/**
 *\brief Send propulsion twist msg.
 *\params const geometry_msgs::Twist propulsion
 *\return
 */
void actuation_send_propulsion_twist(const geometry_msgs::Twist::ConstPtr& propulsion) {

	uint8_t straightCommand[4], turnCommand[4];

	sabertooth_advanced_process_propulsion_twist(&straightCommand[0], &turnCommand[0], propulsion);

	if(actuation_serial && !testing_sabertooth){
		//Check status watchdog
		if(status_msg.level == 0){
			//ROS_INFO("Actuation running");
			
			int bytes = actuation_serial->write(&straightCommand[0], 4);
			ros::Duration(0.01).sleep();
			bytes += actuation_serial->write(&turnCommand[0], 4);
/*
			std::string responce;
			responce = "";
			actuation_serial->read(responce, 1);

			//ROS_INFO("Serial responce after sending: %s\n", responce.c_str());
			if(responce.compare("P") == 0){
				ROS_INFO("Right data send!");	
			}else if(responce.compare("W") == 0){
				ROS_INFO("Wrong data send!");
			}

			actuation_serial->flushInput();
*/	
		}else{
			//ROS_INFO("Actuation not running");
		}
		
	}
}

/**
 *\brief  Send conveyor twist.
 *\params const geometry_msgs::Twist conveyor
 *\return
 */
void actuation_send_conveyor_twist(const geometry_msgs::Twist::ConstPtr& conveyor){
	ROS_INFO("Conveyor message");
	//do nothing
}

/**
 *\brief  Send lighting boolean.
 *\params const std_msgs::Bool lighting
 *\return
 */
void actuation_send_lighting_bool(const std_msgs::Bool::ConstPtr& lighting){
	// do nothing
}
