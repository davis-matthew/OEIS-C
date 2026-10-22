/*
 * A005120  A sixth-order linear divisibility sequence: a(n+6) = -3*a(n+5) -
 *          5*a(n+4) - 5*a(n+3) - 5*a(n+2) - 3*a(n+1) - a(n).
 * https://oeis.org/A005120
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -3*a(n-1) - 5*a(n-2) - 5*a(n-3) - 5*a(n-4) - 3*a(n-5) -
 *            a(n-6), with a(0)=0, a(1)=1, a(2)=-1, a(3)=1, a(4)=-1, a(5)=-1
 * Range:     i8 n <= 14, i16 n <= 27, i32 n <= 54, i64 n <= 107,
 *            i128 n <= 211, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 6);
        cache[0] = 0;
        cache[1] = 1;
        cache[2] = -1;
        cache[3] = 1;
        cache[4] = -1;
        cache[5] = -1;
        cache_len = 6;
    }
    
    if (n < cache_len) {
        return cache[n];
    }
    
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    
    for (idx_t i = cache_len; i <= n; i++) {
        cache[i] = -3 * cache[i-1] - 5 * cache[i-2] - 5 * cache[i-3] 
                   - 5 * cache[i-4] - 3 * cache[i-5] - cache[i-6];
    }
    cache_len = n + 1;
    
    return cache[n];
}
