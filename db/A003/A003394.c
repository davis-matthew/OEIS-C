/*
 * A003394  Numbers that are the sum of 5 positive 9th powers.
 * https://oeis.org/A003394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search -- enumerate all multisets of 5 positive 9th powers with
 *            sum below (B+1)^9 + 4 (complete up to that bound), sort, dedupe;
 *            grow B until at least n terms are known
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 2659, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   A3394_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/A3394_cmp.h"

static uint64_t *A3394_vals = NULL;
static idx_t A3394_cnt = 0;
static idx_t A3394_cap = 0;
static uint64_t A3394_p[128];

static void A3394_push(uint64_t v)
{
	if (A3394_cnt == A3394_cap) {
		idx_t nc = A3394_cap ? A3394_cap * 2 : 65536;
		A3394_vals = (uint64_t *)realloc(A3394_vals, (size_t)nc * sizeof(uint64_t));
		A3394_cap = nc;
	}
	A3394_vals[A3394_cnt++] = v;
}

static void A3394_rec(int k, int minb, int B, uint64_t sum, uint64_t lim)
{
	int b;
	if (k == 0) {
		A3394_push(sum);
		return;
	}
	for (b = minb; b <= B; b++) {
		/* remaining k terms each >= p[b] */
		if (sum + (uint64_t)k * A3394_p[b] > lim) break;
		A3394_rec(k - 1, b, B, sum + A3394_p[b], lim);
	}
}

seq_t a(idx_t n)
{
	static int B = 0;
	static idx_t have = 0;
	int i, j;

	if (A3394_p[1] == 0) {
		for (i = 0; i < 128; i++) {
			uint64_t v = 1;
			for (j = 0; j < 9; j++) v *= (uint64_t)i;
			A3394_p[i] = v;
		}
	}

	while (have < n && B < 110) {
		uint64_t lim;
		idx_t w, r;
		B = B ? B + 4 : 16;
		lim = A3394_p[B + 1] + 3; /* all sums <= lim are found with bases <= B */
		A3394_cnt = 0;
		A3394_rec(5, 1, B, 0, lim);
		qsort(A3394_vals, (size_t)A3394_cnt, sizeof(uint64_t), A3394_cmp);
		w = 0;
		for (r = 0; r < A3394_cnt; r++)
			if (w == 0 || A3394_vals[r] != A3394_vals[w - 1])
				A3394_vals[w++] = A3394_vals[r];
		A3394_cnt = w;
		have = w;
	}

	if (n >= 1 && n <= have) return (seq_t)A3394_vals[n - 1];
	return (seq_t)0;
}
