/*
 * A003439  Number of 6 X 6 stochastic matrices of integers: all rows and
 *          columns sum to n.
 * https://oeis.org/A003439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=0..10} A005467(j) * binomial(5+j+n, 5+2*j). The 11
 *            constants A005467(j) are derived at first call: a(0..10) are
 *            counted exactly by a DP over column partial sums (filling the 6
 *            X 6 matrix one cell at a time), then the formula is back-solved
 *            for them
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 4, u64 n <= 14, u128 n <= 102,
 *            big any n
 * Helpers:   a3439_binom_s, a3439_binom_u, a3439_count, a3439_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a3439_binom_s.h"
#include "../utils/a3439_binom_u.h"
#include "../utils/a3439_count.h"
#include "../utils/a3439_gcd.h"

static uint64_t a3439_coef[11];
static int a3439_ready = 0;

static void a3439_init(void) {
	uint64_t av[11];
	int j, i;
	for (j = 0; j <= 10; j++) av[j] = a3439_count(j);
	for (j = 0; j <= 10; j++) {
		uint64_t s = av[j];
		for (i = 0; i < j; i++)
			s -= a3439_coef[i] * a3439_binom_u((uint64_t)(5 + i + j), (uint64_t)(5 + 2 * i));
		a3439_coef[j] = s;
	}
	a3439_ready = 1;
}

seq_t a(idx_t n) {
	seq_t result = 0;
	idx_t j;
	if (!a3439_ready) a3439_init();
	for (j = 0; j <= 10; j++) {
		if (j > n) break;
		result += (seq_t)a3439_coef[j] * a3439_binom_s(n + 5 + j, 5 + 2 * j);
	}
	return result;
}
