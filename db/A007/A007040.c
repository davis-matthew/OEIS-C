/*
 * A007040  Number of (marked) cyclic n-bit binary strings containing no runs
 *          of length > 2.
 * https://oeis.org/A007040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = a(n-2) + 2*a(n-3) + a(n-4), n >= 7 Base cases: a(1) = 2,
 *            a(2) = 2, a(3) = 6, a(4) = 6, a(5) = 10, a(6) = 20
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n == 1) return 2;
    if (n == 2) return 2;
    if (n == 3) return 6;
    if (n == 4) return 6;
    if (n == 5) return 10;
    if (n == 6) return 20;
    
    if (cache == NULL) {
        cache_len = n + 1;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[1] = 2;
        cache[2] = 2;
        cache[3] = 6;
        cache[4] = 6;
        cache[5] = 10;
        cache[6] = 20;
        
        for (idx_t i = 7; i <= n; i++) {
            cache[i] = cache[i-2] + 2 * cache[i-3] + cache[i-4];
        }
    } else if (n >= cache_len) {
        idx_t old_len = cache_len;
        cache_len = n + 1;
        cache = (seq_t *)realloc(cache, cache_len * sizeof(seq_t));
        
        for (idx_t i = old_len; i <= n; i++) {
            cache[i] = cache[i-2] + 2 * cache[i-3] + cache[i-4];
        }
    }
    
    return cache[n];
}
