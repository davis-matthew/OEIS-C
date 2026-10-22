/*
 * A002569  Max_{k=0..n} { Number of partitions of n into exactly k parts }.
 * https://oeis.org/A002569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Max_{k=0..n} p(n,k), where p(n,k) = number of partitions
 *            of n-k into parts <= k; computed by the DP f_k(m) = f_{k-1}(m) +
 *            f_k(m-k)
 * Range:     u8 n <= 25, u16 n <= 59, u32 n <= 158, u64 n <= 479,
 *            u128 n <= 1593, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
	long long N = (long long)n;
	long long m, k;
	seq_t *f;
	seq_t best;

	if (N == 0) return 1;

	f = (seq_t *)malloc((size_t)(N + 1) * sizeof(seq_t));
	if (!f) return 0;
	for (m = 0; m <= N; m++) f[m] = 0;
	f[0] = 1;

	best = 0;
	for (k = 1; k <= N; k++) {
		/* f[m] = partitions of m into parts <= k, needed only for m <= N-k */
		for (m = k; m <= N - k; m++)
			f[m] += f[m - k];
		if (f[N - k] > best) best = f[N - k];
	}

	free(f);
	return best;
}
