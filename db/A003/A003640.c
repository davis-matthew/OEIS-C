/*
 * A003640  Number of genera of imaginary quadratic field with discriminant
 *          -k, k = A003657(n).
 * https://oeis.org/A003640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2^(omega(A003657(n)) - 1); A003657 (fundamental
 *            discriminants -k) found by a search
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 121 terms through n = 1000 (2026-11-01)
 * Helpers:   omega, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/omega.h"
#include "../utils/pow2.h"

static bool sqfree_u64(uint64_t n) {
	if (n == 0) return false;
	for (uint64_t i = 2; i * i <= n; i++) {
		if (n % (i * i) == 0) return false;
	}
	return true;
}

static bool in_A003657(uint64_t k) {
	if (k % 4 == 3) return sqfree_u64(k);
	if (k % 4 == 0) {
		uint64_t m = k / 4;
		if (m % 4 != 1 && m % 4 != 2) return false;
		return sqfree_u64(m);
	}
	return false;
}

seq_t a(idx_t n) {
	static uint64_t *cache = NULL;
	static idx_t cache_size = 0;

	if (n > cache_size) {
		idx_t new_size = (n < 1000) ? 2000 : (n * 2);
		uint64_t *nc = (uint64_t *)malloc(new_size * sizeof(uint64_t));
		if (cache) {
			memcpy(nc, cache, cache_size * sizeof(uint64_t));
			free(cache);
		}
		cache = nc;

		uint64_t k = (cache_size > 0) ? cache[cache_size - 1] + 1 : 1;
		idx_t count = cache_size;
		while (count < new_size) {
			if (in_A003657(k)) cache[count++] = k;
			k++;
		}
		cache_size = new_size;
	}

	int w = omega(cache[n - 1]);
	return pow2((idx_t)(w - 1));
}
