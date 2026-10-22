/*
 * A001922  Numbers k such that 3*k^2 - 3*k + 1 is both a square (A000290) and
 *          a centered hexagonal number (A003215).
 * https://oeis.org/A001922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) - 15*a(n-2) + a(n-3), a(0)=1, a(1)=8, a(2)=105
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 33,
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
        cache_len = 3;
        cache = (seq_t *)malloc(3 * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 8;
        cache[2] = 105;
    }
    
    if (n >= cache_len) {
        idx_t old_len = cache_len;
        cache_len = n + 1;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        
        for (idx_t i = old_len; i <= n; i++) {
            cache[i] = 15 * cache[i-1] - 15 * cache[i-2] + cache[i-3];
        }
    }
    
    return cache[n];
}
