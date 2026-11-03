/*
 * A007939  Some permutation of digits is a cube.
 * https://oeis.org/A007939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   search: n is a term iff some cube has the same nonzero digits as
 *            n and no more zeros (leading zeros allowed)
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 67 terms through n = 914 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

static size_t hslot(uint64_t key, size_t size) {
	uint64_t h = key * 0x9E3779B97F4A7C15ULL;
	return (size_t)(h >> 20) & (size - 1);
}

/* key = nonzero digits sorted descending; returns digit count */
static int signature(uint64_t x, uint64_t *key, int *zeros) {
	int cnt[10];
	int d = 0, dg, r;
	uint64_t k = 0;
	memset(cnt, 0, sizeof(cnt));
	do {
		cnt[x % 10]++;
		x /= 10;
		d++;
	} while (x > 0);
	for (dg = 9; dg >= 1; dg--)
		for (r = 0; r < cnt[dg]; r++) k = k * 10 + (uint64_t)dg;
	*key = k;
	*zeros = cnt[0];
	return d;
}

static uint64_t *terms = NULL;
static idx_t tcount = 0, tcap = 0;
static uint64_t next_num = 1;

static uint64_t *hkeys = NULL;
static int *hzeros = NULL;
static size_t hsize = 0, hused = 0;

static int cubes_digits = 0;
static uint64_t next_base = 1;

static void hinsert_raw(uint64_t *keys, int *zs, size_t size, uint64_t k1, int z) {
	size_t i = hslot(k1, size);
	while (keys[i] != 0 && keys[i] != k1) i = (i + 1) & (size - 1);
	if (keys[i] == 0) { keys[i] = k1; zs[i] = z; hused++; }
	else if (z < zs[i]) zs[i] = z;
}

static void hrehash(size_t nsize) {
	uint64_t *nk = (uint64_t *)calloc(nsize, sizeof(uint64_t));
	int *nz = (int *)calloc(nsize, sizeof(int));
	size_t i;
	hused = 0;
	for (i = 0; i < hsize; i++)
		if (hkeys[i] != 0) hinsert_raw(nk, nz, nsize, hkeys[i], hzeros[i]);
	free(hkeys);
	free(hzeros);
	hkeys = nk;
	hzeros = nz;
	hsize = nsize;
}

static void hinsert(uint64_t key, int z) {
	if (hsize == 0) {
		hsize = 1024;
		hkeys = (uint64_t *)calloc(hsize, sizeof(uint64_t));
		hzeros = (int *)calloc(hsize, sizeof(int));
	}
	if ((hused + 1) * 2 > hsize) hrehash(hsize * 2);
	hinsert_raw(hkeys, hzeros, hsize, key + 1, z);
}

static int hlookup(uint64_t key) {
	size_t i;
	uint64_t k1 = key + 1;
	if (hsize == 0) return -1;
	i = hslot(k1, hsize);
	while (hkeys[i] != 0) {
		if (hkeys[i] == k1) return hzeros[i];
		i = (i + 1) & (hsize - 1);
	}
	return -1;
}

static void ensure_cubes(int d) {
	uint64_t limit = 1;
	int i;
	if (d <= cubes_digits) return;
	for (i = 0; i < d; i++) limit *= 10;
	for (;;) {
		uint64_t c = next_base * next_base * next_base;
		uint64_t key;
		int z;
		if (c >= limit) break;
		signature(c, &key, &z);
		hinsert(key, z);
		next_base++;
	}
	cubes_digits = d;
}

seq_t a(idx_t n) {
	while (tcount < n) {
		uint64_t key;
		int z, d, best;
		d = signature(next_num, &key, &z);
		ensure_cubes(d);
		best = hlookup(key);
		if (best >= 0 && best <= z) {
			if (tcount >= tcap) {
				tcap = tcap ? tcap * 2 : 1024;
				terms = (uint64_t *)realloc(terms, (size_t)tcap * sizeof(uint64_t));
			}
			terms[tcount++] = next_num;
		}
		next_num++;
	}
	return (seq_t)terms[n - 1];
}
