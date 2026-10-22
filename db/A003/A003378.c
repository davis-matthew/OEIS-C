/*
 * A003378  Numbers that are the sum of 11 positive 7th powers.
 * https://oeis.org/A003378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search: enumerate all multisets of 11 positive 7th powers with
 *            sum <= X (doubling X as needed), sort and dedupe; a(n) is the
 *            n-th smallest distinct sum
 * Range:     u8 n <= 2, u16 n <= 243, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a3378_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a3378_cmp.h"

static uint64_t *a3378_vals = NULL;
static size_t a3378_cnt = 0, a3378_cap = 0;
static uint64_t a3378_pw[64];

static void a3378_push(uint64_t v) {
	if (a3378_cnt == a3378_cap) {
		size_t nc = a3378_cap ? a3378_cap * 2 : 4096;
		a3378_vals = (uint64_t *)realloc(a3378_vals, nc * sizeof(uint64_t));
		a3378_cap = nc;
	}
	a3378_vals[a3378_cnt++] = v;
}

static void a3378_rec(int k, int minb, uint64_t sum, uint64_t X) {
	int b;
	if (k == 0) {
		a3378_push(sum);
		return;
	}
	for (b = minb; b < 64; b++) {
		uint64_t p = a3378_pw[b];
		if (sum + (uint64_t)k * p > X) break;
		a3378_rec(k - 1, b, sum + p, X);
	}
}

seq_t a(idx_t n) {
	static uint64_t X = 0;
	static size_t distinct = 0;
	int b;
	if (a3378_pw[1] == 0) {
		for (b = 0; b < 64; b++) {
			uint64_t p = 1;
			int e;
			for (e = 0; e < 7; e++) p *= (uint64_t)b;
			a3378_pw[b] = p;
		}
	}
	while (X == 0 || distinct < n) {
		size_t i, j;
		X = X ? X * 2 : ((uint64_t)1 << 20);
		a3378_cnt = 0;
		a3378_rec(11, 1, 0, X);
		qsort(a3378_vals, a3378_cnt, sizeof(uint64_t), a3378_cmp);
		j = 0;
		for (i = 0; i < a3378_cnt; i++)
			if (j == 0 || a3378_vals[i] != a3378_vals[j - 1])
				a3378_vals[j++] = a3378_vals[i];
		a3378_cnt = j;
		distinct = j;
	}
	return (seq_t)a3378_vals[n - 1];
}
