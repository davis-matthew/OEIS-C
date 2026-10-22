/*
 * A001636  A Fielder sequence: a(n) = a(n-1) + a(n-2) - a(n-7), n >= 8.
 * https://oeis.org/A001636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + a(n-3) + a(n-4) + a(n-5) + a(n-6), n >= 7
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 49, u64 n <= 98, u128 n <= 196,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
	static seq_t *cache = NULL;
	static idx_t cache_size = 0;

	if (n == 1) return 0;
	if (n == 2) return 2;
	if (n == 3) return 3;
	if (n == 4) return 6;
	if (n == 5) return 10;
	if (n == 6) return 17;

	if (cache == NULL) {
		cache_size = n + 1;
		cache = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
		cache[1] = 0;
		cache[2] = 2;
		cache[3] = 3;
		cache[4] = 6;
		cache[5] = 10;
		cache[6] = 17;
		for (idx_t i = 7; i <= n; i++) {
			cache[i] = cache[i-2] + cache[i-3] + cache[i-4] + cache[i-5] + cache[i-6];
		}
		return cache[n];
	}

	if (n < cache_size) {
		return cache[n];
	}

	seq_t *new_cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
	cache = new_cache;

	for (idx_t i = cache_size; i <= n; i++) {
		cache[i] = cache[i-2] + cache[i-3] + cache[i-4] + cache[i-5] + cache[i-6];
	}
	cache_size = n + 1;

	return cache[n];
}
