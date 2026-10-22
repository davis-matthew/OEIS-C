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
 * Helpers:   A001832_pow2, in_A003657, omega_u64, sqfree_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/A001832_pow2.h"
#include "../utils/in_A003657.h"
#include "../utils/omega_u64.h"
#include "../utils/sqfree_u64.h"

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

	int w = omega_u64(cache[n - 1]);
	return A001832_pow2((idx_t)(w - 1));
}
