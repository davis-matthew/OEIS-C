/*
 * A001235  Taxi-cab numbers: sums of 2 cubes in more than 1 way.
 * https://oeis.org/A001235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   search -- enumerate x^3 + y^3 (1 <= x <= y) window by window in
 *            increasing value order; a(n) is the n-th value hit by two or
 *            more pairs
 * Range:     u8 none, u16 n <= 9, u32 n <= 3134, u64 n <= 9157,
 *            u128 n <= 9157, big any n
 * Helpers:   a1235_cmp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a1235_cmp.h"

#define A1235_HB 20
#define A1235_HSIZE ((size_t)1 << A1235_HB)

static uint64_t *a1235_terms = NULL;
static size_t a1235_nterms = 0, a1235_capterms = 0;
static uint64_t *a1235_ycur = NULL;
static uint64_t a1235_nx = 0, a1235_capx = 0;
static uint64_t a1235_L = 1, a1235_W = 4096;
static uint64_t *a1235_hkey = NULL;
static unsigned char *a1235_hcnt = NULL;
static uint32_t *a1235_used = NULL;

static void a1235_push(uint64_t v) {
	if (a1235_nterms == a1235_capterms) {
		size_t nc = a1235_capterms ? 2 * a1235_capterms : 1024;
		a1235_terms = (uint64_t *)realloc(a1235_terms, nc * sizeof(uint64_t));
		a1235_capterms = nc;
	}
	a1235_terms[a1235_nterms++] = v;
}

static void a1235_window(void) {
	uint64_t L = a1235_L, R = L + a1235_W;
	size_t nused = 0, start = a1235_nterms, i;
	uint64_t mask = A1235_HSIZE - 1, x;
	(void)L;
	for (x = 1;; x++) {
		uint64_t x3 = x * x * x, y;
		if (2 * x3 >= R) break;
		if (x > a1235_nx) {
			if (x >= a1235_capx) {
				uint64_t nc = a1235_capx ? 2 * a1235_capx : 1024;
				a1235_ycur = (uint64_t *)realloc(a1235_ycur, (size_t)nc * sizeof(uint64_t));
				a1235_capx = nc;
			}
			a1235_ycur[x] = x;
			a1235_nx = x;
		}
		y = a1235_ycur[x];
		for (;;) {
			uint64_t v = x3 + y * y * y, h;
			if (v >= R) break;
			h = (v * 0x9E3779B97F4A7C15ULL) >> (64 - A1235_HB);
			while (a1235_hkey[h] != 0 && a1235_hkey[h] != v) h = (h + 1) & mask;
			if (a1235_hkey[h] == 0) {
				a1235_hkey[h] = v;
				a1235_hcnt[h] = 1;
				a1235_used[nused++] = (uint32_t)h;
			} else {
				if (a1235_hcnt[h] == 1) a1235_push(v);
				if (a1235_hcnt[h] < 255) a1235_hcnt[h]++;
			}
			y++;
		}
		a1235_ycur[x] = y;
	}
	for (i = 0; i < nused; i++) a1235_hkey[a1235_used[i]] = 0;
	if (a1235_nterms > start)
		qsort(a1235_terms + start, a1235_nterms - start, sizeof(uint64_t), a1235_cmp);
	a1235_L = R;
	if (nused < ((size_t)1 << 15)) a1235_W *= 2;
	else if (nused > ((size_t)1 << 17) && a1235_W > 1) a1235_W /= 2;
}

seq_t a(idx_t n) {
	if (n < 1) return 0;
	if (a1235_hkey == NULL) {
		a1235_hkey = (uint64_t *)calloc(A1235_HSIZE, sizeof(uint64_t));
		a1235_hcnt = (unsigned char *)calloc(A1235_HSIZE, 1);
		a1235_used = (uint32_t *)malloc(A1235_HSIZE * sizeof(uint32_t));
	}
	while ((idx_t)a1235_nterms < n) a1235_window();
	return (seq_t)a1235_terms[n - 1];
}
