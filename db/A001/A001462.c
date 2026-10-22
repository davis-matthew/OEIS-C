/*
 * A001462  Golomb's sequence: a(n) is the number of times n occurs in the
 *          sequence, starting with a(1) = 1.
 * https://oeis.org/A001462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice, core
 * Formula:   a(1) = 1; a(n+1) = 1 + a(n+1-a(a(n)))
 * Range:     u8 n <= 5834, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_computed_up_to = 0;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    
    if (cache == NULL) {
        idx_t initial_size = n + 1000;
        if (initial_size < 100000) initial_size = 100000;
        cache_len = initial_size;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[1] = 1;
        cache_computed_up_to = 1;
    }
    
    while (n >= cache_len) {
        idx_t new_len = cache_len * 2;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        for (idx_t i = 0; i < cache_len; ++i) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }
    
    if (n > cache_computed_up_to) {
        for (idx_t i = cache_computed_up_to + 1; i <= n; ++i) {
            seq_t a_i_minus_1 = cache[i - 1];
            idx_t a_idx = (idx_t)a_i_minus_1;
            seq_t a_a_i_minus_1 = cache[a_idx];
            idx_t inner_idx = i - (idx_t)a_a_i_minus_1;
            cache[i] = 1 + cache[inner_idx];
        }
        cache_computed_up_to = n;
    }
    
    return cache[n];
}
