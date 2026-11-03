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
 * Validated: standard, pass, 33 terms through n = 984 (2026-11-01)
 * Helpers:   binomial, binomial_u64, gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/binomial_u64.h"
#include "../utils/gcd_u64.h"

static uint64_t a3439_count(int N) {
	size_t B = (size_t)N + 1, size = 1, i;
	int c, r;
	uint64_t *f, res;
	unsigned char *tot;
	for (c = 0; c < 6; c++) size *= B;
	f = (uint64_t *)calloc(size, sizeof(uint64_t));
	tot = (unsigned char *)malloc(size);
	tot[0] = 0;
	for (i = 1; i < size; i++) tot[i] = (unsigned char)(i % B + tot[i / B]);
	f[0] = 1;
	for (r = 0; r < 6; r++) {
		unsigned lim = (unsigned)((r + 1) * N);
		size_t stride = 1;
		for (c = 0; c < 6; c++) {
			size_t base, d, k;
			for (base = 0; base < size; base += stride * B) {
				for (d = 0; d < B; d++) {
					for (k = 0; k < stride; k++) {
						size_t idx = base + d * stride + k;
						if (tot[idx] > lim) { f[idx] = 0; continue; }
						if (d > 0) f[idx] += f[idx - stride];
					}
				}
			}
			stride *= B;
		}
		for (i = 0; i < size; i++)
			if (tot[i] != lim) f[i] = 0;
	}
	res = f[size - 1];
	free(f);
	free(tot);
	return res;
}

static uint64_t a3439_coef[11];
static int a3439_ready = 0;

static void a3439_init(void) {
	uint64_t av[11];
	int j, i;
	for (j = 0; j <= 10; j++) av[j] = a3439_count(j);
	for (j = 0; j <= 10; j++) {
		uint64_t s = av[j];
		for (i = 0; i < j; i++)
			s -= a3439_coef[i] * binomial_u64((uint64_t)(5 + i + j), (uint64_t)(5 + 2 * i));
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
		result += (seq_t)a3439_coef[j] * binomial(n + 5 + j, 5 + 2 * j);
	}
	return result;
}
