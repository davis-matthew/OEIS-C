/*
 * A003239  Number of rooted planar trees with n non-root nodes: circularly
 *          cycling the subtrees at the root gives equivalent trees.
 * https://oeis.org/A003239
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (1/(2n)) * Sum_{d|n} phi(n/d)*binomial(2d,d) for n > 0,
 *            a(0) = 1
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 20, u64 n <= 36, u128 n <= 69,
 *            big any n
 * Helpers:   a003239_fixed, a003239_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a003239_fixed.h"
#include "../utils/a003239_phi.h"

seq_t a(idx_t n)
{
	if (n == 0) return 1;

	if (!a003239_fixed()) {
		/* arbitrary precision: direct formula */
		seq_t c = 1, sum = 0;
		idx_t k;
		for (k = 1; k <= n; k++) {
			c = c * 2 * (seq_t)(2 * k - 1);
			c = c / (seq_t)k;            /* c = binomial(2k,k) */
			if (n % k == 0)
				sum += c * (seq_t)a003239_phi((uint64_t)(n / k));
		}
		return sum / ((seq_t)n * 2);
	}

	/* fixed width: values overflow long before this */
	if (n > 200) return 0;

	/* Work modulo n*2^w: value = q*n + r, q wraps in seq_t, 0 <= r < n.
	   Pascal triangle (additions only) gives binomial(2d-1,d). */
	{
		idx_t rows = 2 * n;              /* rows 0 .. 2n-1 */
		seq_t *Pq = (seq_t *)malloc(sizeof(seq_t) * (size_t)(rows + 1));
		uint64_t *Pr = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)(rows + 1));
		seq_t Q = 0;
		uint64_t R = 0;
		idx_t m, j;
		for (j = 0; j <= rows; j++) { Pq[j] = 0; Pr[j] = 0; }
		Pq[0] = (seq_t)(1 / n);
		Pr[0] = 1 % n;
		for (m = 1; m < rows; m++) {
			for (j = m; j >= 1; j--) {
				uint64_t r = Pr[j] + Pr[j - 1];
				seq_t q = Pq[j] + Pq[j - 1];
				if (r >= n) { r -= n; q += 1; }
				Pq[j] = q;
				Pr[j] = r;
			}
			/* row m = 2d-1 -> binomial(2d-1, d) */
			if (m % 2 == 1) {
				idx_t d = (m + 1) / 2;
				if (n % d == 0) {
					uint64_t ph = a003239_phi((uint64_t)(n / d));
					seq_t q = Pq[d];
					uint64_t rr = Pr[d] * ph;
					q *= (seq_t)ph;
					q += (seq_t)(rr / n);
					Q += q;
					R += rr % n;
				}
			}
		}
		Q += (seq_t)(R / n);
		free(Pq);
		free(Pr);
		return Q;
	}
}
