/*
 * A000897  a(n) = (4*n)! / ((2*n)!*n!^2).
 * https://oeis.org/A000897
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) * 4*(4n-3)*(4n-1) / n^2, a(0) = 1 (equivalently
 *            a(n) = (4n)!/((2n)!*n!^2))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   a897_gcd_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a897_gcd_u.h"

seq_t a(idx_t n)
{
	seq_t r = 1;
	idx_t k;
	for (k = 1; k <= n; k++) {
		uint64_t c = (uint64_t)4 * (4 * k - 3) * (4 * k - 1);
		uint64_t d = (uint64_t)k * k;
		uint64_t g = a897_gcd_u(c, d);
		c /= g;
		d /= g;
		/* c and d are coprime and r*c/d is an integer, so d divides r */
		r /= d;
		r *= c;
	}
	return r;
}
