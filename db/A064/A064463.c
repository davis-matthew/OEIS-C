/*
 * A064463  Numbers k such that A001414(k) is a square and sets a new record
 *          for squares.
 * https://oeis.org/A064463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   search over squares s = m^2. For each s, find f(s) = min{k :
 *            A001414(k) = s}; a term is an f(s) smaller than f(s') for every
 *            larger square s'. Squares are scanned from the top down with a
 *            running minimum. f(s) >= 2(s-2), so the only candidates below
 *            4(s-4) have the form p*q with p, q prime and p+...
 * Range:     u8 n <= 7, u16 n <= 44, u32 n <= 600, u64 n <= 600,
 *            u128 n <= 600, big any n
 * Validated: standard, pass, 57 terms through n = 587 (2026-11-07)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_u64.h"

static uint64_t *A064463_terms = 0;
static size_t A064463_cnt = 0;
static uint64_t A064463_M = 0;

static uint64_t A064463_sopfr(uint64_t k)
{
	uint64_t r = 0, p;
	if (k < 2) return 0;
	while (k % 2 == 0) { k /= 2; r += 2; }
	for (p = 3; p <= k / p; p += 2)
		while (k % p == 0) { k /= p; r += p; }
	if (k > 1) r += k;
	return r;
}

static uint64_t A064463_brute(uint64_t s, uint64_t from, uint64_t to)
{
	uint64_t k;
	for (k = from; k < to; k++)
		if (A064463_sopfr(k) == s) return k;
	return 0;
}

static void A064463_build(uint64_t M)
{
	uint64_t *tmp = (uint64_t *)malloc((size_t)(M + 2) * sizeof(uint64_t));
	size_t c = 0, i;
	uint64_t cur = 2 * ((M + 1) * (M + 1) - 2);
	uint64_t m = M;
	if (!tmp) return;
	for (;;) {
		uint64_t s = m * m, f = 0;
		if (m == 0) {
			f = 1;
		} else if (m == 1) {
			f = 0;
		} else if (s <= 10000) {
			uint64_t lo = 2 * (s - 2);
			if (lo < 2) lo = 2;
			if (lo < cur) f = A064463_brute(s, lo, cur);
		} else if (2 * (s - 2) < cur) {
			uint64_t b4 = 4 * (s - 4);
			uint64_t lim = cur < b4 ? cur : b4;
			uint64_t p = 2;
			while (p <= s / 2 && p * (s - p) < lim) {
				if (is_prime_u64(s - p)) { f = p * (s - p); break; }
				if (p == 2) p = 3;
				else { p += 2; while (!is_prime_u64(p)) p += 2; }
			}
			if (!f && b4 < cur) f = A064463_brute(s, b4, cur);
		}
		if (f && f < cur) {
			tmp[c++] = f;
			cur = f;
		}
		if (m == 0) break;
		m--;
	}
	free(A064463_terms);
	A064463_terms = (uint64_t *)malloc((c ? c : 1) * sizeof(uint64_t));
	for (i = 0; i < c; i++) A064463_terms[i] = tmp[c - 1 - i];
	A064463_cnt = c;
	free(tmp);
}

seq_t a(idx_t n)
{
	if (A064463_M == 0) {
		A064463_M = 256;
		A064463_build(A064463_M);
	}
	while ((uint64_t)n > A064463_cnt) {
		A064463_M *= 2;
		A064463_build(A064463_M);
	}
	return (seq_t)A064463_terms[n - 1];
}
