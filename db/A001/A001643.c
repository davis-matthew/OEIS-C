/*
 * A001643  A Fielder sequence.
 * https://oeis.org/A001643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-4) + a(n-5) + a(n-6), a(1) = 1,
 *            a(2) = 3, a(3) = 4, a(4) = 11, a(5) = 21, a(6) = 42 (from G.f.
 *            x*(1+2*x+4*x^3+5*x^4+6*x^5)/(1-x-x^2-x^4-x^5-x^6))
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 72, u128 n <= 145,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
	seq_t v0 = 1, v1 = 3, v2 = 4, v3 = 11, v4 = 21, v5 = 42;
	idx_t i;
	if (n <= 1) return v0;
	if (n == 2) return v1;
	if (n == 3) return v2;
	if (n == 4) return v3;
	if (n == 5) return v4;
	if (n == 6) return v5;
	for (i = 7; i <= n; i++) {
		seq_t nx = v5 + v4 + v2 + v1 + v0;
		v0 = v1;
		v1 = v2;
		v2 = v3;
		v3 = v4;
		v4 = v5;
		v5 = nx;
	}
	return v5;
}
