/*
 * A003748  Number of 2-factors in K_5 X P_n.
 * https://oeis.org/A003748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 47*a(n-1) + 288*a(n-2) - 436*a(n-3), with a(1) = 12, a(2)
 *            = 814, a(3) = 41278
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)12;
    if (n == 2) return (seq_t)814;
    if (n == 3) return (seq_t)41278;
    
    if (cache == NULL) {
        cache_len = n + 1;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        for (idx_t i = 0; i < cache_len; i++) {
            cache[i] = 0;
        }
        cache[1] = (seq_t)12;
        cache[2] = (seq_t)814;
        cache[3] = (seq_t)41278;
    } else if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        for (idx_t i = 0; i < cache_len; i++) {
            new_cache[i] = cache[i];
        }
        for (idx_t i = cache_len; i < new_len; i++) {
            new_cache[i] = 0;
        }
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }
    
    for (idx_t i = 4; i <= n; i++) {
        if (cache[i] == 0) {
            cache[i] = (seq_t)47 * cache[i-1] + (seq_t)288 * cache[i-2] - (seq_t)436 * cache[i-3];
        }
    }
    
    return cache[n];
}
