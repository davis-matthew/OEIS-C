/*
 * A002058  Number of internal triangles in all triangulations of an
 *          (n+1)-gon.
 * https://oeis.org/A002058
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*binomial(2*n-5,n-5)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   a002058_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a002058_gcd.h"

seq_t a(idx_t n)
{
	/* binomial(2n-5, n-5) = prod_{i=1}^{n-5} (n+i)/i, computed exactly */
	seq_t r = 1;
	idx_t k = n - 5;
	idx_t i;
	for (i = 1; i <= k; i++) {
		seq_t num = (seq_t)n + (seq_t)i;
		seq_t den = (seq_t)i;
		seq_t g = a002058_gcd(r, den);
		r /= g;
		den /= g;
		num /= den;
		r *= num;
	}
	return r * 2;
}
