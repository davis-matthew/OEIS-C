/*
 * A002622  Number of partitions of at most n into at most 5 parts.
 * https://oeis.org/A002622
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f. 1/((1-x)^2*(1-x^2)*(1-x^3)*(1-x^4)*(1-x^5)), computed by
 *            coin-change DP with parts 1,1,2,3,4,5
 * Range:     u8 n <= 13, u16 n <= 54, u32 n <= 565, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
	static const int parts[6] = { 1, 1, 2, 3, 4, 5 };
	seq_t *f = (seq_t *)malloc(((size_t)n + 1) * sizeof(seq_t));
	seq_t r;
	idx_t i;
	int k;
	f[0] = 1;
	for (i = 1; i <= n; i++)
		f[i] = 0;
	for (k = 0; k < 6; k++) {
		idx_t p = (idx_t)parts[k];
		for (i = p; i <= n; i++)
			f[i] += f[i - p];
	}
	r = f[n];
	free(f);
	return r;
}
