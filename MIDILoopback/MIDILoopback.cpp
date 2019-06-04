// MIDILoopback.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include "RtMidi.h"



std::vector<RtMidiOut*> outputPorts;

void mycallback(double deltatime, std::vector< unsigned char > *message, void *userData)
{
	unsigned int nBytes = message->size();
	for (unsigned int i = 0; i < nBytes; i++)
		std::cout << "Byte " << i << " = " << (int)message->at(i) << ", ";
	if (nBytes > 0)
		std::cout << "stamp = " << deltatime << std::endl;



	for (int i = 0; i < outputPorts.size(); i++) {
		outputPorts[i]->sendMessage(message);
	}



}




int main()
{
    

	RtMidiIn  *midiin = 0;
	
	// RtMidiIn constructor
	try {
		midiin = new RtMidiIn();
	}
	catch (RtMidiError &error) {
		error.printMessage();
		exit(EXIT_FAILURE);
	}
	// Check inputs.
	unsigned int nPorts = midiin->getPortCount();
	std::cout << "\nThere are " << nPorts << " MIDI input sources available.\n";
	std::string portName;
	for (unsigned int i = 0; i < nPorts; i++) {
		try {
			portName = midiin->getPortName(i);
		}
		catch (RtMidiError &error) {
			error.printMessage();
		}
		std::cout << "  Input Port #" << i << ": " << portName << '\n';
	}

	int inputPort = 0;

	std::cout << "Select MIDI input port: ";
	std::cin >> inputPort;

	midiin->openPort(inputPort);



	RtMidiOut *midiout = 0;
	// RtMidiOut constructor
	try {
		midiout = new RtMidiOut();
	}
	catch (RtMidiError &error) {
		error.printMessage();
		exit(EXIT_FAILURE);
	}
	// Check outputs.
	nPorts = midiout->getPortCount();
	std::cout << "\nThere are " << nPorts << " MIDI output ports available.\n";

	

	std::vector<int> loopBackPorts;

	for (unsigned int i = 0; i < nPorts; i++) {
		try {
			portName = midiout->getPortName(i);
			
			if (portName.substr(0,5) == "Loop_")
				loopBackPorts.push_back(i);

		}
		catch (RtMidiError &error) {
			error.printMessage();
		}
		std::cout << "  Output Port #" << i << ": " << portName << '\n';
	}
	std::cout << '\n';

	std::cout << "Automatic port selection with prefix Loop_" << std::endl;

	for (int i = 0; i < loopBackPorts.size(); i++) {
		std::cout << "port: " << loopBackPorts[i] << std::endl;

		RtMidiOut *outPort;
		try {
			outPort = new RtMidiOut();
		}
		catch (RtMidiError &error) {
			error.printMessage();
			exit(EXIT_FAILURE);
		}

		outputPorts.push_back(outPort);

		outPort->openPort(loopBackPorts[i]);

	}




	midiin->setCallback(mycallback);
	midiin->ignoreTypes(false, false, false);

	std::cout << "\nReading MIDI input ... press <enter> to quit.\n";
	char input;
	std::cin.get(input);

	std::cin.ignore();
	std::cin.ignore();


	// Clean up

	delete midiin;
	delete midiout;

	for (int i = 0; i < outputPorts.size(); i++) {
		delete outputPorts[i];
	}



	return 0;


}
