/*
 * A003601  Numbers j such that the average of the divisors of j is an
 *          integer: sigma_0(j) divides sigma_1(j). Alternatively, numbers j
 *          such that tau(j) (A000005(j)) divides sigma(j) (A000203(j)).
 * https://oeis.org/A003601
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   search for numbers j where tau(j) divides sigma(j)
 * Range:     u8 n <= 70, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   compute_sigma
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/compute_sigma.h"

static seq_t compute_tau(seq_t n) {
	seq_t count = 0;
	for (seq_t d = 1; d * d <= n; d++) {
		if (n % d == 0) {
			count++;
			if (d * d != n) {
				count++;
			}
		}
	}
	return count;
}

seq_t a(idx_t n) {
	seq_t count = 0;
	seq_t j = 0;

	while (count < (seq_t)n) {
		j++;
		seq_t sigma = compute_sigma(j);
		seq_t tau = compute_tau(j);

		if (sigma % tau == 0) {
			count++;
		}
	}

	return j;
}
