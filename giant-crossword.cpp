/* developer's notes!!!
- the coordinate system is x rightwards, y downwards (i.e. not Cartesian, more Vulkan / graphics)
- where direction is represented by number-like, 0 is horizontal, 1 is vertical. x goes before y
*/

// include
#include <iostream>
#include <chrono>
#include <unordered_map>
#include <map>
#include <vector>
#define SprsCharMat std::map< int, std::map< int, char > >
#define SplitSprsCharMat std::unordered_map< char, SprsCharMat >
#define IntPair std::pair<int32_t, int32_t>
#define IntPairVector std::vector<IntPair>
#define PairArraySized std::pair<std::pair<int32_t, int32_t>*, uint32_t>
#define nullchar '\x00'

/// rng utility functions
// using set seed (1) or changing seed (unix timestamp microseconds)
uint64_t state = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
//uint64_t state = 1;

// ultra-fast xorshift pseudo-rng (uhh idk why this works)
inline
uint64_t random_64() {
	state ^= state >> 12; // a
	state ^= state << 25; // b
	state ^= state >> 27; // c
	return state * 2685821657736338717ULL;
}

// random float uniformly distributed between 0 and 1
inline
float random() {
	return (double)random_64() / UINT64_MAX;
}

// random variable with a {prob} chance of 1, else 0
inline
bool random_bin(float prob) {
	return random() < prob;
}

// random variable with a 50% chance of 0 or 1
inline
bool random_bin() {
	return random_64() % 2;
}

/// initialize empty data structures
SprsCharMat matrix;
SplitSprsCharMat split_matrix;

/// constants
const int32_t window_radius = 10; // chebyshev distance of window edge char from center. note to self: use window_radius to determine future center chars
const float density = 0.15; // proportion of window edge chars to be filled before considered complete

/// logical utility functions

// returns coordinates of all 
IntPairVector snapshot(const SprsCharMat& mat, int32_t x, int32_t y) {
	IntPairVector coords;
	auto xlitr = mat.lower_bound(x - window_radius);
	const auto xritr = mat.upper_bound(x + window_radius);
	for (xlitr; xlitr != xritr; xlitr++) {
		const auto column = xlitr->second;
		auto yuitr = column.lower_bound(y - window_radius);
		const auto ybitr = column.upper_bound(y + window_radius);
		for (yuitr; yuitr != ybitr; yuitr++)
			coords.push_back({ xlitr->first, yuitr->first });
	}
	return coords;
}

inline
IntPairVector snapshot(char c, int32_t x, int32_t y) {
	return snapshot(split_matrix[c], x, y);
}

// accepts a coordinate, gives the contiguous across word as char* containing that coordinate (or empty char* if not exists)
inline
std::string across_search(int32_t x, int32_t y) {
	std::string str;
	while ((bool)matrix[x][y])
		x--;
	x++;
	for (int32_t i = x; (bool)matrix[i][y]; i++) {
		std::cout << matrix[i][y];
	}
	return str;
}

// accepts a coordinate, gives the contiguous down word as char* containing that coordinate (or empty char* if not exists)
inline
char* down_search(int32_t x, int32_t y) {

}

int main() {
	matrix[-2][0] = 'c';
	matrix[-1][0] = 'h';
	matrix[0][0] = 'a';
	//std::cout << across_search(0, 0);
	IntPairVector vec = snapshot(matrix, 8, 1);
	for (auto i : vec) {
		std::cout << across_search(i.first, i.second) << std::endl;
	}
}
