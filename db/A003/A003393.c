/*
 * A003393  Numbers that are the sum of 4 positive 9th powers.
 * https://oeis.org/A003393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   search. Enumerate every b1<=b2<=b3<=b4 with bi <= M, keep the
 *            sums b1^9+b2^9+b3^9+b4^9 that are below (M+1)^9+3 (all smaller
 *            terms are found that way), then sort and remove duplicates. M is
 *            increased until there are at least n terms
 * Range:     u8 n <= 1, u16 n <= 14, u32 n <= 925, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

typedef unsigned __int128 a3393_u128;

static a3393_u128 *a3393_cache = NULL;
static idx_t a3393_cache_len = 0;

static a3393_u128 a3393_p9(uint64_t x) {
	a3393_u128 v = x;
	a3393_u128 x2 = v * v;
	a3393_u128 x4 = x2 * x2;
	return x4 * x4 * v;
}

static int a3393_cmp(const void *p, const void *q) {
	a3393_u128 x = *(const a3393_u128 *)p;
	a3393_u128 y = *(const a3393_u128 *)q;
	if (x < y) return -1;
	if (x > y) return 1;
	return 0;
}

static seq_t a3393_to_seq(a3393_u128 v) {
	uint64_t hi = (uint64_t)(v >> 64);
	uint64_t lo = (uint64_t)v;
	seq_t r;
	if (hi == 0) return (seq_t)lo;
	r = (seq_t)hi;
	r = r * (seq_t)4294967296ULL;
	r = r * (seq_t)4294967296ULL;
	r += (seq_t)lo;
	return r;
}

seq_t a(idx_t n) {
	uint64_t M = 30;
	if (n <= a3393_cache_len) return a3393_to_seq(a3393_cache[n - 1]);

	for (;;) {
		a3393_u128 *pw = (a3393_u128 *)malloc(sizeof(a3393_u128) * (size_t)(M + 1));
		a3393_u128 limit = a3393_p9(M + 1) + 3;
		size_t cap = 1024, len = 0, k, dl;
		a3393_u128 *buf = (a3393_u128 *)malloc(sizeof(a3393_u128) * cap);
		uint64_t b1, b2, b3, b4;

		for (k = 0; k <= M; k++) pw[k] = a3393_p9((uint64_t)k);

		for (b1 = 1; b1 <= M; b1++) {
			a3393_u128 s1 = pw[b1];
			if (s1 + 3 * pw[b1] >= limit) break;
			for (b2 = b1; b2 <= M; b2++) {
				a3393_u128 s2 = s1 + pw[b2];
				if (s2 + 2 * pw[b2] >= limit) break;
				for (b3 = b2; b3 <= M; b3++) {
					a3393_u128 s3 = s2 + pw[b3];
					if (s3 + pw[b3] >= limit) break;
					for (b4 = b3; b4 <= M; b4++) {
						a3393_u128 s4 = s3 + pw[b4];
						if (s4 >= limit) break;
						if (len == cap) {
							cap *= 2;
							buf = (a3393_u128 *)realloc(buf, sizeof(a3393_u128) * cap);
						}
						buf[len++] = s4;
					}
				}
			}
		}
		free(pw);

		qsort(buf, len, sizeof(a3393_u128), a3393_cmp);
		dl = 0;
		for (k = 0; k < len; k++) {
			if (dl == 0 || buf[k] != buf[dl - 1]) buf[dl++] = buf[k];
		}

		if ((idx_t)dl >= n) {
			if (a3393_cache != NULL) free(a3393_cache);
			a3393_cache = buf;
			a3393_cache_len = (idx_t)dl;
			return a3393_to_seq(a3393_cache[n - 1]);
		}
		free(buf);
		M = M + M / 2 + 1;
	}
}
