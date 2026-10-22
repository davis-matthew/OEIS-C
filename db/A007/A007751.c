/*
 * A007751  Even bisection of A007750.
 * https://oeis.org/A007751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-1) - a(n-2) + 8, a(0) = 0, a(1) = 7
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t computed_up_to = 0;
    static bool initialized = false;
    
    if (n >= cache_len) {
        idx_t new_len = (n + 1) * 2;
        if (new_len < 10) new_len = 10;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        
        if (cache != NULL) {
            for (idx_t i = 0; i < cache_len; i++) {
                new_cache[i] = cache[i];
            }
            free(cache);
        }
        
        cache = new_cache;
        cache_len = new_len;
    }
    
    if (!initialized) {
        cache[0] = 0;
        computed_up_to = 0;
        initialized = true;
    }
    
    if (n > computed_up_to) {
        for (idx_t i = computed_up_to + 1; i <= n; i++) {
            if (i == 1) {
                cache[1] = 7;
            } else {
                cache[i] = 16 * cache[i-1] - cache[i-2] + 8;
            }
        }
        computed_up_to = n;
    }
    
    return cache[n];
}
