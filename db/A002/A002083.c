/*
 * A002083  Narayana-Zidek-Capell numbers: a(n) = 1 for n <= 2. Otherwise
 *          a(2n) = 2a(2n-1), a(2n+1) = 2a(2n) - a(n).
 * https://oeis.org/A002083
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, core, nonn, nice
 * Formula:   a(1) = 1, a(2) = 1; a(2n) = 2*a(2n-1), a(2n+1) = 2*a(2n) - a(n)
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
	// Expand cache if needed
	if (n >= cache_len) {
		idx_t new_len = (n < 100) ? 100 : (n + 100);
		seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
		cache = new_cache;
		for (idx_t i = cache_len; i < new_len; i++) {
			cache[i] = 0;
		}
		cache_len = new_len;
	}
	
	if (n == 1 || n == 2) {
		return 1;
	}
	
	if (cache[n] != 0) {
		return cache[n];
	}
	
	seq_t result;
	if (n % 2 == 0) {
		result = 2 * a(n - 1);
	} else {
		result = 2 * a(n - 1) - a((n - 1) / 2);
	}
	
	cache[n] = result;
	return result;
}
