/*
 * A002739  a(n) = ((2*n-1)!/(2*n!*(n-2)!))*((n^3-3*n^2+2*n+2)/(n^2-1)).
 * https://oeis.org/A002739
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/4)*(n^3 - 3*n^2 + 2*n + 2)*A000108(n), where
 *            A000108(n) = C(2n,n)/(n+1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 930 (2026-11-01)
 * Helpers:   catalan, gcd_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/catalan.h"
#include "../utils/gcd_idx.h"

seq_t a(idx_t n) {
	seq_t ns = (seq_t)n;
	/* n^3 - 3n^2 + 2n + 2 = n(n-1)(n-2) + 2 */
	seq_t poly = ns * (ns - 1) * (ns - 2) + 2;
	seq_t cat = catalan(n);

	idx_t g = 1;
	if (poly % 4 == 0) g = 4;
	else if (poly % 2 == 0) g = 2;
	idx_t r = 4 / g;

	poly /= (seq_t)g;
	cat /= (seq_t)r;
	return poly * cat;
}
