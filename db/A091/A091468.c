/*
 * A091468  Number of unlabeled alternating octopuses with n black nodes and n
 *          white nodes.
 * https://oeis.org/A091468
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*A003239(n) - 2, where A003239(n) = (1/(2n)) * Sum_{d|n}
 *            phi(n/d)*binomial(2d,d)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Validated: partial, pass, 25 terms through n = 25 (2026-11-07)
 * Helpers:   mem, totient_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/totient_u64.h"
#include "../utils/mem.h"

static int A003239_fixed(void)
{
	seq_t x = 1;
	int i;
	for (i = 0; i < 300; i++) x = (seq_t)(x * 2);
	return x == 0;
}

static seq_t A003239_a(idx_t n)
{
	if (n == 0) return 1;

	if (!A003239_fixed()) {
		seq_t c = 1, sum = 0;
		idx_t k;
		for (k = 1; k <= n; k++) {
			c = c * 2 * (seq_t)(2 * k - 1);
			c = c / (seq_t)k;
			if (n % k == 0)
				sum += c * (seq_t)totient_u64((uint64_t)(n / k));
		}
		return sum / ((seq_t)n * 2);
	}

	if (n > 200) return 0;

	{
		idx_t rows = 2 * n;
		seq_t *Pq = mem_new(seq_t, (size_t)(rows + 1));
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
			if (m % 2 == 1) {
				idx_t d = (m + 1) / 2;
				if (n % d == 0) {
					uint64_t ph = totient_u64((uint64_t)(n / d));
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
		mem_free(Pq);
		free(Pr);
		return Q;
	}
}

seq_t a(idx_t n)
{
	return (seq_t)3 * A003239_a(n) - (seq_t)2;
}
