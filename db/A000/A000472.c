/*
 * A000472  a(n) = a(n-1)^2 + (a(n-2) + 1)*(a(n-1) - a(n-2)^2).
 * https://oeis.org/A000472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = a(n-1)^2 + (a(n-2) + 1)*(a(n-1) - a(n-2)^2), a(1) = 2,
 *            a(2) = 5
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    
    if (n >= cache_len) {
        idx_t new_len = (cache_len == 0) ? 10 : cache_len;
        while (new_len <= n) new_len *= 2;
        
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (cache != NULL) {
            for (idx_t i = 0; i < cache_len; i++) {
                new_cache[i] = cache[i];
            }
            free(cache);
        }
        for (idx_t i = cache_len; i < new_len; i++) {
            new_cache[i] = 0;
        }
        cache = new_cache;
        cache_len = new_len;
    }
    
    if (cache[n] != 0) {
        return cache[n];
    }
    
    seq_t a_n_minus_1 = a((idx_t)(n - 1));
    seq_t a_n_minus_2 = a((idx_t)(n - 2));
    
    seq_t result = a_n_minus_1 * a_n_minus_1 + 
                   (a_n_minus_2 + (seq_t)1) * (a_n_minus_1 - a_n_minus_2 * a_n_minus_2);
    
    cache[n] = result;
    return result;
}
