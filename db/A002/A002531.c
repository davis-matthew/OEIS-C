/*
 * A002531  a(2*n) = a(2*n-1) + a(2*n-2), a(2*n+1) = 2*a(2*n) + a(2*n-1); a(0)
 *          = a(1) = 1.
 * https://oeis.org/A002531
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy, core, nice
 * Formula:   a(2*n) = a(2*n-1) + a(2*n-2), a(2*n+1) = 2*a(2*n) + a(2*n-1),
 *            a(0) = a(1) = 1
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 34, u64 n <= 68, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0 || n == 1) return 1;
    
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    
    if (cache == NULL) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 1;
        cache_len = 2;
    }
    
    if (n >= cache_len) {
        cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        for (idx_t i = cache_len; i <= n; i++) {
            if (i % 2 == 0) {
                cache[i] = cache[i-1] + cache[i-2];
            } else {
                cache[i] = 2 * cache[i-1] + cache[i-2];
            }
        }
        cache_len = n + 1;
    }
    
    return cache[n];
}
