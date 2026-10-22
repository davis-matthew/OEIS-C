/*
 * A002466  A jumping problem.
 * https://oeis.org/A002466
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-5) + 2*a(n-10) for n > 10; a(1)=a(2)=1, a(3)=2,
 *            a(4)=4, a(5)=7, a(6)=13, a(7)=17, a(8)=30, a(9)=60, a(10)=107
 * Range:     u8 n <= 11, u16 n <= 22, u32 n <= 42, u64 n <= 83, u128 n <= 164,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_capacity = 0;

seq_t a(idx_t n) {
	static seq_t base[] = {0, 1, 1, 2, 4, 7, 13, 17, 30, 60, 107};

	if (n <= 10) {
		return base[n];
	}

	if (cache == NULL) {
		cache_capacity = (n < 1000) ? 1000 : n + 100;
		cache = (seq_t *)malloc(sizeof(seq_t) * (cache_capacity + 1));
		for (idx_t i = 0; i <= 10; i++) {
			cache[i] = base[i];
		}
		for (idx_t i = 11; i <= cache_capacity; i++) {
			cache[i] = (seq_t)15 * cache[i - 5] + (seq_t)2 * cache[i - 10];
		}
	} else if (n > cache_capacity) {
		idx_t new_capacity = n + 100;
		cache = (seq_t *)realloc(cache, sizeof(seq_t) * (new_capacity + 1));
		for (idx_t i = cache_capacity + 1; i <= new_capacity; i++) {
			cache[i] = (seq_t)15 * cache[i - 5] + (seq_t)2 * cache[i - 10];
		}
		cache_capacity = new_capacity;
	}

	return cache[n];
}
