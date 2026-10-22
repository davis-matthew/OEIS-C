/*
 * A006192  Number of nonintersecting (or self-avoiding) rook paths joining
 *          opposite corners of 3 X n board.
 * https://oeis.org/A006192
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, walk, nice, easy
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) + 2*a(n-3) + a(n-4), a(0) = 0, a(1) =
 *            1, a(2) = 4, a(3) = 12
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 38, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    
    if (cache == NULL) {
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = 4;
        cache[3] = 12;
        cache_len = 4;
    }
    
    if (n < cache_len) {
        return cache[n];
    }
    
    idx_t old_len = cache_len;
    cache_len = n + 1;
    cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
    
    for (idx_t i = old_len; i < cache_len; i++) {
        cache[i] = 4 * cache[i-1] - 3 * cache[i-2] + 2 * cache[i-3] + cache[i-4];
    }
    
    return cache[n];
}
