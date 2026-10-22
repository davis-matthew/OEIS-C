/*
 * A006495  Real part of (1 + 2*i)^n, where i is sqrt(-1).
 * https://oeis.org/A006495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*a(n-1) - 5*a(n-2), a(0) = 1, a(1) = 1
 * Range:     i8 n <= 7, i16 n <= 13, i32 n <= 27, i64 n <= 55, i128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 1;
    
    if (cache == NULL) {
        cache_len = 2;
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 1;
    }
    
    if (n >= cache_len) {
        idx_t new_len = cache_len;
        while (new_len <= n) new_len *= 2;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        for (idx_t i = 0; i < cache_len; i++) {
            new_cache[i] = cache[i];
        }
        for (idx_t i = cache_len; i < new_len; i++) {
            new_cache[i] = 2 * new_cache[i-1] - 5 * new_cache[i-2];
        }
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }
    
    return cache[n];
}
