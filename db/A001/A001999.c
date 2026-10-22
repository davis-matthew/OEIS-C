/*
 * A001999  a(n) = a(n-1)*(a(n-1)^2 - 3).
 * https://oeis.org/A001999
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(n-1)*(a(n-1)^2 - 3), a(0) = 3
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t* cache = NULL;
static idx_t cache_capacity = 0;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (cache == NULL) {
        cache_capacity = n + 100;
        if (cache_capacity < 100) cache_capacity = 100;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_capacity);
        cache[0] = 3;
        cache_len = 1;
    } else if (n >= cache_capacity) {
        cache_capacity = n + 100;
        seq_t* new_cache = (seq_t*)malloc(sizeof(seq_t) * cache_capacity);
        for (idx_t i = 0; i < cache_len; i++) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
    }
    
    while (cache_len <= n) {
        idx_t i = cache_len;
        seq_t prev = cache[i - 1];
        seq_t sq = prev * prev;
        cache[i] = prev * (sq - 3);
        cache_len++;
    }
    
    return cache[n];
}
