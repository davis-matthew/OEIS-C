/*
 * A004293  Expansion of (1+2*x+x^2)/(1-26*x+x^2).
 * https://oeis.org/A004293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 26*a(n-1) - a(n-2), a(0) = 1, a(1) = 28, a(2) = 728
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    
    // Base cases
    if (n == 0) return 1;
    if (n == 1) return 28;
    if (n == 2) return 728;
    
    // Check if we already have the value
    if (cache != NULL && n < cache_len) {
        return cache[n];
    }
    
    // Need to compute
    if (cache == NULL) {
        cache_len = n + 1;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 28;
        cache[2] = 728;
        for (idx_t i = 3; i <= n; i++) {
            cache[i] = 26 * cache[i-1] - cache[i-2];
        }
    } else if (n >= cache_len) {
        idx_t old_len = cache_len;
        cache_len = n + 1;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        for (idx_t i = old_len; i <= n; i++) {
            cache[i] = 26 * cache[i-1] - cache[i-2];
        }
    }
    
    return cache[n];
}
