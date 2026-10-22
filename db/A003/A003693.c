/*
 * A003693  Number of 2-factors in P_4 X P_n.
 * https://oeis.org/A003693
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 7*a(n-2) - 2*a(n-3) - 3*a(n-4) + a(n-5), n > 5
 *            a(1) = 0, a(2) = 2, a(3) = 3, a(4) = 18, a(5) = 54
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t computed_up_to = 0;
    
    if (n < 1) return 0;
    
    if (cache == NULL) {
        cache_size = (n < 1000) ? 1000 : n + 1000;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;
        
        cache[1] = 0;
        cache[2] = 2;
        cache[3] = 3;
        cache[4] = 18;
        cache[5] = 54;
        computed_up_to = 5;
    }
    
    if (n >= cache_size) {
        idx_t new_size = n + 1000;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_cache) return 0;
        
        for (idx_t i = 1; i <= computed_up_to; i++) {
            new_cache[i] = cache[i];
        }
        
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }
    
    if (n > computed_up_to) {
        for (idx_t i = computed_up_to + 1; i <= n; i++) {
            cache[i] = 2*cache[i-1] + 7*cache[i-2] - 2*cache[i-3] - 3*cache[i-4] + cache[i-5];
        }
        computed_up_to = n;
    }
    
    return cache[n];
}
