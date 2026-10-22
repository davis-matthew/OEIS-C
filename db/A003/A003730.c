/*
 * A003730  Number of 2-factors in C_5 X P_n.
 * https://oeis.org/A003730
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*a(n-1) - 4*a(n-2) - 22*a(n-3) + 3*a(n-4), n > 4 with
 *            a(1) = 1, a(2) = 11, a(3) = 81, a(4) = 666
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n <= 0) return 0;
    
    if (cache == NULL) {
        cache_len = 5;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 11;
        cache[3] = 81;
        cache[4] = 666;
    }
    
    if (n < cache_len) {
        return cache[n];
    }
    
    idx_t old_len = cache_len;
    cache_len = n + 1;
    cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
    
    for (idx_t i = old_len; i < cache_len; i++) {
        cache[i] = 9 * cache[i-1] - 4 * cache[i-2] - 22 * cache[i-3] + 3 * cache[i-4];
    }
    
    return cache[n];
}
