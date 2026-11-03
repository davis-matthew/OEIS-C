/*
 * A005303  Representation degeneracies for Ramond strings.
 * https://oeis.org/A005303
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f. Product (1 - x^k)^(-c(k)), c(k) = 0, 2, 4, 3, 4, 2, 4, 2,
 *            4, 2, ... a(n) = (1/n) * ∑_{k=1}^{n} b(k) * a(n-k), where b(k) =
 *            ∑_{d|k} c(d) * d
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 32 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/mem.h"

static idx_t c_coeff(idx_t k) {
	if (k == 1) return 0;
	if (k == 2) return 2;
	if (k == 3) return 4;
	if (k == 4) return 3;
	if (k == 5) return 4;
	if (k == 6) return 2;
	return (k % 2 == 1) ? 4 : 2;
}

static seq_t b_coeff(idx_t m) {
	seq_t result = 0;
	for (idx_t d = 1; d * d <= m; d++) {
		if (m % d == 0) {
			result += (seq_t)c_coeff(d) * (seq_t)d;
			if (d != m / d) {
				idx_t d2 = m / d;
				result += (seq_t)c_coeff(d2) * (seq_t)d2;
			}
		}
	}
	return result;
}

seq_t a(idx_t n) {
	static seq_t *cache = NULL;
	static bool *computed = NULL;
	static idx_t cache_size = 0;

	if (n == 0) return 1;

	if (cache_size == 0) {
		cache_size = 256;
		cache = mem_new(seq_t, cache_size);
		computed = (bool *)malloc(cache_size * sizeof(bool));
		cache[0] = 1;
		computed[0] = true;
		for (idx_t i = 1; i < cache_size; i++) {
			computed[i] = false;
		}
	}

	if (n >= cache_size) {
		idx_t new_size = n + 256;
		seq_t *new_cache = mem_new(seq_t, new_size);
		bool *new_computed = (bool *)malloc(new_size * sizeof(bool));
		mem_copy(new_cache, cache, cache_size * sizeof(seq_t));
		memcpy(new_computed, computed, cache_size * sizeof(bool));
		mem_free(cache);
		free(computed);
		cache = new_cache;
		computed = new_computed;
		for (idx_t i = cache_size; i < new_size; i++) {
			computed[i] = false;
		}
		cache_size = new_size;
	}

	for (idx_t i = 1; i <= n; i++) {
		if (!computed[i]) {
			seq_t sum = 0;
			for (idx_t k = 1; k <= i; k++) {
				sum += b_coeff(k) * cache[i - k];
			}
			cache[i] = sum / (seq_t)i;
			computed[i] = true;
		}
	}

	return cache[n];
}
