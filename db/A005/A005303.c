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
 * Helpers:   b_coeff, c_coeff
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/b_coeff.h"
#include "../utils/c_coeff.h"

seq_t a(idx_t n) {
	static seq_t *cache = NULL;
	static bool *computed = NULL;
	static idx_t cache_size = 0;

	if (n == 0) return 1;

	if (cache_size == 0) {
		cache_size = 256;
		cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
		computed = (bool *)malloc(cache_size * sizeof(bool));
		cache[0] = 1;
		computed[0] = true;
		for (idx_t i = 1; i < cache_size; i++) {
			computed[i] = false;
		}
	}

	if (n >= cache_size) {
		idx_t new_size = n + 256;
		seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
		bool *new_computed = (bool *)malloc(new_size * sizeof(bool));
		memcpy(new_cache, cache, cache_size * sizeof(seq_t));
		memcpy(new_computed, computed, cache_size * sizeof(bool));
		free(cache);
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
