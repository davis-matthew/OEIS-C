/*
 * A006054  a(n) = 2*a(n-1) + a(n-2) - a(n-3), with a(0) = a(1) = 0, a(2) = 1.
 * https://oeis.org/A006054
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - a(n-3), a(0) = 0, a(1) = 0, a(2) = 1
 * Range:     u8 n <= 8, u16 n <= 15, u32 n <= 29, u64 n <= 56, u128 n <= 111,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t max_computed = 2;

seq_t a(idx_t n) {
    if (n <= 2) {
        return (n == 2) ? 1 : 0;
    }
    
    if (cache == NULL) {
        cache_len = (n < 1000) ? 1000 : n + 100;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 1;
    } else if (n >= cache_len) {
        idx_t new_len = n + 100;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        for (idx_t i = 0; i < cache_len; i++) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }
    
    for (idx_t i = max_computed + 1; i <= n; i++) {
        cache[i] = 2 * cache[i-1] + cache[i-2] - cache[i-3];
    }
    max_computed = (n > max_computed) ? n : max_computed;
    
    return cache[n];
}
