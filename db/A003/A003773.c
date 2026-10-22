/*
 * A003773  Number of spanning trees in K_4 X P_n.
 * https://oeis.org/A003773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 204*a(n-1) - 1190*a(n-2) + 204*a(n-3) - a(n-4), with a(1)
 *            = 16, a(2) = 3456, a(3) = 686000, a(4) = 135834624
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
	if (n == 1) return (seq_t)16;
	if (n == 2) return (seq_t)3456;
	if (n == 3) return (seq_t)686000;
	if (n == 4) return (seq_t)135834624;

	static seq_t *cache = NULL;
	static idx_t cache_len = 0;
	static idx_t computed_up_to = 4;

	if (cache == NULL) {
		cache_len = 100;
		cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
		cache[0] = (seq_t)16;
		cache[1] = (seq_t)3456;
		cache[2] = (seq_t)686000;
		cache[3] = (seq_t)135834624;
	}

	if (n >= cache_len) {
		idx_t new_len = cache_len;
		while (new_len <= n) new_len *= 2;
		cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
		cache_len = new_len;
	}

	for (idx_t i = computed_up_to + 1; i <= n; i++) {
		cache[i-1] = 204 * cache[i-2] - 1190 * cache[i-3] + 204 * cache[i-4] - cache[i-5];
	}
	if (n > computed_up_to) computed_up_to = n;

	return cache[n-1];
}
