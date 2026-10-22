/*
 * A006743  Number of convex polygons of length 2n on honeycomb, or EG-convex
 *          polyominoes.
 * https://oeis.org/A006743
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   (n-5)*a(n) = 2*(n-5)*a(n-1) + (7*n-47)*a(n-2) + 12*(7-n)*a(n-3)
 *            + 4*(29-4*n)*a(n-4) + 16*(n-8)*a(n-5) + 16*(n-8)*a(n-6), for n
 *            >= 10; base cases a(3)=1, a(4)=0, a(5)=3, a(6)=2, a(7)=10,
 *            a(8)=14, a(9)=40
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 29, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   get_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/get_base.h"

static seq_t *cache = NULL;
static bool *computed = NULL;
static idx_t cache_size = 0;

static void ensure_computed(idx_t n) {
	if (n < 10) return;
	
	if (cache == NULL) {
		cache_size = (n > 100) ? n + 100 : 100;
		cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
		computed = (bool *)calloc(cache_size, sizeof(bool));
		
		cache[3] = 1;
		cache[4] = 0;
		cache[5] = 3;
		cache[6] = 2;
		cache[7] = 10;
		cache[8] = 14;
		cache[9] = 40;
		
		for (idx_t i = 3; i < 10; i++) {
			computed[i] = true;
		}
	} else if (n >= cache_size) {
		idx_t old_size = cache_size;
		cache_size = n + 100;
		cache = (seq_t *)realloc(cache, cache_size * sizeof(seq_t));
		computed = (bool *)realloc(computed, cache_size * sizeof(bool));
		memset(cache + old_size, 0, (cache_size - old_size) * sizeof(seq_t));
		memset(computed + old_size, 0, (cache_size - old_size) * sizeof(bool));
	}
	
	for (idx_t i = 10; i <= n; i++) {
		if (computed[i]) continue;
		
		seq_t a_i_1 = (i - 1 < 10) ? get_base(i - 1) : cache[i - 1];
		seq_t a_i_2 = (i - 2 < 10) ? get_base(i - 2) : cache[i - 2];
		seq_t a_i_3 = (i - 3 < 10) ? get_base(i - 3) : cache[i - 3];
		seq_t a_i_4 = (i - 4 < 10) ? get_base(i - 4) : cache[i - 4];
		seq_t a_i_5 = (i - 5 < 10) ? get_base(i - 5) : cache[i - 5];
		seq_t a_i_6 = (i - 6 < 10) ? get_base(i - 6) : cache[i - 6];
		
		seq_t si = (seq_t)i;
		seq_t term1 = 2 * (si - 5) * a_i_1;
		seq_t term2 = (7 * si - 47) * a_i_2;
		seq_t term3 = 12 * (7 - si) * a_i_3;
		seq_t term4 = 4 * (29 - 4 * si) * a_i_4;
		seq_t term5 = 16 * (si - 8) * a_i_5;
		seq_t term6 = 16 * (si - 8) * a_i_6;
		
		seq_t sum = term1 + term2 + term3 + term4 + term5 + term6;
		cache[i] = sum / (si - 5);
		computed[i] = true;
	}
}

seq_t a(idx_t n) {
	if (n < 3) return 0;
	
	if (n < 10) {
		return get_base(n);
	}
	
	ensure_computed(n);
	return cache[n];
}
