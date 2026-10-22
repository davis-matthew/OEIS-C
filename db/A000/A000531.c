/*
 * A000531  From area of cyclic polygon of 2n + 1 sides.
 * https://oeis.org/A000531
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = ((2n+1)!/((n!)^2) - 4^n)/2 = (2n+1)*binomial(2n-1, n-1) -
 *            2^(2n-1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   a000531_g
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000531_g.h"

seq_t a(idx_t n)
{
	seq_t b = 1, p = 1, r;
	idx_t i;

	if (n < 1) return 0;

	/* b = binomial(n-1+i, i) for i = 0..n-1, ending at binomial(2n-1, n-1) */
	for (i = 1; i < n; i++) {
		unsigned long long num = (unsigned long long)(n + i);
		unsigned long long den = (unsigned long long)i;
		unsigned long long g = a000531_g((unsigned long long)(b % (seq_t)den), den);
		b = (seq_t)(b / (seq_t)g);
		b = (seq_t)(b * (seq_t)(num / (den / g)));
	}

	for (i = 0; i + 1 < 2 * n; i++)
		p = (seq_t)(p * 2);

	r = (seq_t)(b * (seq_t)(2 * n + 1));
	r = (seq_t)(r - p);
	return r;
}
