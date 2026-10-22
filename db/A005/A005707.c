/*
 * A005707  a(1) = a(2) = a(3) = a(4) = 1, a(n) = a(a(n-1))+a(n-a(n-1)) for n
 *          >= 5.
 * https://oeis.org/A005707
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(1) = a(2) = a(3) = a(4) = 1, a(n) = a(a(n-1)) + a(n - a(n-1))
 *            for n >= 5
 * Range:     u8 n <= 742, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n < 1) return 0;
    
    if (cache == NULL) {
        cache_len = (n < 4) ? 4 : n;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 1;
        
        for (idx_t i = 5; i <= n; i++) {
            seq_t a_i_minus_1 = cache[i - 2];
            seq_t a_of_a_i_minus_1 = cache[(idx_t)(a_i_minus_1 - 1)];
            seq_t a_i_minus_a_i_minus_1 = cache[i - 1 - (idx_t)a_i_minus_1];
            cache[i - 1] = a_of_a_i_minus_1 + a_i_minus_a_i_minus_1;
        }
    } else if (n > cache_len) {
        seq_t *old_cache = cache;
        idx_t old_len = cache_len;
        cache_len = n;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        
        for (idx_t i = 0; i < old_len; i++) {
            cache[i] = old_cache[i];
        }
        free(old_cache);
        
        for (idx_t i = old_len + 1; i <= n; i++) {
            seq_t a_i_minus_1 = cache[i - 2];
            seq_t a_of_a_i_minus_1 = cache[(idx_t)(a_i_minus_1 - 1)];
            seq_t a_i_minus_a_i_minus_1 = cache[i - 1 - (idx_t)a_i_minus_1];
            cache[i - 1] = a_of_a_i_minus_1 + a_i_minus_a_i_minus_1;
        }
    }
    
    return cache[n - 1];
}
