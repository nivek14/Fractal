#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include "fractal.hpp"
#include <omp.h>

using namespace std;

Fractal::Fractal() {
	
	ifstream fin(ArquivoInput);
	if(!fin) {
		cout << "Não foi possível abrir o arquivo!" << endl;
		cin.ignore();	
	}

	//Manipulando a entrada de dados
	fin >> imageWidth >> imageHeight >> maxN;
	fin >> minR >> maxR >> minI >> maxI;
	fin >> ColorR >> ColorG >> ColorB;
	fin >> fractalModel; 
	fin.close();
	if(ColorR >= 257 || ColorG >= 257 || ColorB >= 257 || ColorR < 1 || ColorG < 1 || ColorB < 1 ) {
		cout << "Valor max/min de RBG ultrapassado, entre com números >= 0 || <= 256" << endl;
		cin.ignore();
		EXIT_FAILURE;
	}
}

double Fractal::mapToReal(int x) {
	double range = maxR - minR;
	return x * (range / imageWidth) + minR;
}

double Fractal::mapToImaginary(int y) {
	double range = maxI - minI;
	return y * (range / imageHeight) + minI;
}

int Fractal::gerarFractal(double cr, double ci, int max_iterations, double NumberR, double NumberI) {
	
	int i;
	int threads = atoi(argv[4]);
	double zr = cr;
	double zi = ci;
	
	# pragma omp parallel num_threads (threads)  \
		private (i);
	
	# pragma omp for
	for(i=0;i < max_iterations && zr * zr + zi * zi < 4.0;i++) {
	double temp = 2.0 * zr * zi;
		zr = zr * zr - zi * zi + NumberR;
		zi = temp + NumberI;
	}
	
	return i;
}

ImageTwo::ImageTwo() {
	
	if(fractalModel == "Two") {
		ofstream fout("Two.ppm");
		fout << "P3" << endl; // Magic Number extensão ppm
		fout << imageWidth << " " << imageHeight << endl;
		fout << "256" << endl; // Valor máximo de cor do pixel RGB
		
		for(int y = 0; y < imageWidth; ++y) {
			for(int x = 0; x < imageHeight; ++x) {
				double cr = mapToReal(x);
				double ci = mapToImaginary(y);
				double NumberR = cr;
				double NumberI = ci;
				int n = gerarFractal(cr, ci, maxN, NumberR, NumberI);

				int r = (n  % ColorR); // CORES
				int g = (n * 3 % ColorG);
				int b = (n % ColorB);

				fout << r << " " << g << " " << b << " ";
			}
				fout << endl;
		}
				fout.close();
				cout << "Terminado! " << fractalModel << " gerado com sucesso!" << endl;
	}
}

ImageOne::ImageOne() {
	
	if(fractalModel == "One") {
		ofstream fout("One.ppm");
		fout << "P3" << endl; // Magic Number extensão ppm
		fout << imageWidth << " " << imageHeight << endl;
		fout << "256" << endl; // Valor máximo de cor do pixel RGB
		for(int y = 0; y < imageHeight; ++y) {
			for(int x = 0; x < imageWidth; ++x) {
				double cr = mapToReal(x);
				double ci = mapToImaginary(y);
				double NumberR = -0.70176;
				double NumberI = -0.3842;
				int n = gerarFractal(cr, ci, maxN, NumberR, NumberI);

				int r = (n  % ColorR); // CORES
				int g = (n * 3 % ColorG);
				int b = (n % ColorB);

				fout << r << " " << g << " " << b << " ";
			}
				fout << endl;
		}
				fout.close();
				cout << "Terminado! " << fractalModel << " gerado com sucesso!" << endl;
	}
}
