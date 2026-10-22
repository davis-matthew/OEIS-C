/*
 * A002524  Number of permutations of length n within distance 2 of a fixed
 *          permutation.
 * https://oeis.org/A002524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-3) - a(n-5) for n >= 5 Initial values:
 *            a(0) = 1, a(1) = 1, a(2) = 2, a(3) = 6, a(4) = 14
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 27, u64 n <= 53, u128 n <= 105,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (cache == NULL) {
        cache_len = (n >= 128) ? (n + 1) : 128;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 2;
        cache[3] = 6;
        cache[4] = 14;
        
        for (idx_t i = 5; i < cache_len; i++) {
            cache[i] = (seq_t)2 * cache[i-1] + (seq_t)2 * cache[i-3] - cache[i-5];
        }
    } else if (n >= cache_len) {
        idx_t old_len = cache_len;
        cache_len = n + 1;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        
        for (idx_t i = old_len; i < cache_len; i++) {
            cache[i] = (seq_t)2 * cache[i-1] + (seq_t)2 * cache[i-3] - cache[i-5];
        }
    }
    
    return cache[n];
}
