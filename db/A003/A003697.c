/*
 * A003697  Duplicate of A006253.
 * https://oeis.org/A003697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 4*a(n-1) - a(n-2) + 2 (n even, n >= 2), a(n) = 4*a(n-1) -
 *            a(n-2) - 2 (n odd, n >= 3), a(0) = 1, a(1) = 2
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 13, i64 n <= 13, i128 n <= 13,
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
    if (n == 1) return 2;
    
    if (cache == NULL || n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        
        idx_t start_idx = 0;
        if (cache != NULL) {
            for (idx_t i = 0; i < cache_len; i++) {
                new_cache[i] = cache[i];
            }
            free(cache);
            start_idx = cache_len;
        } else {
            new_cache[0] = 1;
            new_cache[1] = 2;
            start_idx = 2;
        }
        
        for (idx_t i = start_idx; i <= n; i++) {
            if (i % 2 == 0) {
                new_cache[i] = 4 * new_cache[i-1] - new_cache[i-2] + 2;
            } else {
                new_cache[i] = 4 * new_cache[i-1] - new_cache[i-2] - 2;
            }
        }
        
        cache = new_cache;
        cache_len = new_len;
    }
    
    return cache[n];
}
