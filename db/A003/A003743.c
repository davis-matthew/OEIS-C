/*
 * A003743  Number of Hamiltonian cycles in O_5 X P_n.
 * https://oeis.org/A003743
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-1) + 136*a(n-2) - 460*a(n-3) + 432*a(n-4) +
 *            256*a(n-5), a(1) = 6, a(2) = 204, a(3) = 4152, a(4) = 90012,
 *            a(5) = 1916640, a(6) = 41086080
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t max_computed = 0;

seq_t a(idx_t n)
{
	if (n < 1) return 0;
	
	if (n == 1) return 6;
	if (n == 2) return 204;
	if (n == 3) return 4152;
	if (n == 4) return 90012;
	if (n == 5) return 1916640;
	if (n == 6) return 41086080;
	
	if (cache == NULL) {
		cache_len = (n < 100) ? 100 : n + 10;
		cache = (seq_t *)malloc(sizeof(seq_t) * (cache_len + 1));
		cache[1] = 6;
		cache[2] = 204;
		cache[3] = 4152;
		cache[4] = 90012;
		cache[5] = 1916640;
		cache[6] = 41086080;
		max_computed = 6;
	} else if (n > cache_len) {
		cache_len = n + 10;
		cache = (seq_t *)realloc(cache, sizeof(seq_t) * (cache_len + 1));
	}
	
	for (idx_t i = max_computed + 1; i <= n; i++) {
		cache[i] = 16 * cache[i-1] + 136 * cache[i-2] 
		         - 460 * cache[i-3] + 432 * cache[i-4] 
		         + 256 * cache[i-5];
	}
	
	if (n > max_computed) {
		max_computed = n;
	}
	
	return cache[n];
}
