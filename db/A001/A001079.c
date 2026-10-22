/*
 * A001079  a(n) = 10*a(n-1) - a(n-2); a(0) = 1, a(1) = 5.
 * https://oeis.org/A001079
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - a(n-2), a(0) = 1, a(1) = 5
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    
    if (n == 0) return 1;
    if (n == 1) return 5;
    
    if (cache == NULL || n >= cache_len) {
        idx_t new_len = (n < 1000) ? 1000 : n + 100;
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
            new_cache[1] = 5;
            start_idx = 2;
        }
        
        for (idx_t i = start_idx; i < new_len; i++) {
            new_cache[i] = 10 * new_cache[i-1] - new_cache[i-2];
        }
        
        cache = new_cache;
        cache_len = new_len;
    }
    
    return cache[n];
}
