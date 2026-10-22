/*
 * A000955  A sequence satisfying (a(2n+1) + 1)^3 = Sum_{k=1..2n+1} a(k)^3.
 * https://oeis.org/A000955
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=6, a(3)=8, a(2n+2) = 3*a(2n+1)^2 + 8*a(2n+1) + 6,
 *            a(2n+3) = 3*a(2n+1)^3 + 12*a(2n+1)^2 + 17*a(2n+1) + 8
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#define CACHE_SIZE 10001

static seq_t cache[CACHE_SIZE];
static uint8_t cached[CACHE_SIZE];

seq_t a(idx_t n) {
	if (n < CACHE_SIZE && cached[n]) return cache[n];
	
	seq_t result;
	if (n == 1) {
		result = 1;
	} else if (n == 2) {
		result = 6;
	} else if (n == 3) {
		result = 8;
	} else if (n % 2 == 0) {
		seq_t am1 = a(n - 1);
		result = 3 * am1 * am1 + 8 * am1 + 6;
	} else {
		seq_t am2 = a(n - 2);
		result = 3 * am2 * am2 * am2 + 12 * am2 * am2 + 17 * am2 + 8;
	}
	
	if (n < CACHE_SIZE) {
		cache[n] = result;
		cached[n] = 1;
	}
	
	return result;
}
