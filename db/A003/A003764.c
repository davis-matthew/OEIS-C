/*
 * A003764  Number of 2-factors in W_4 X P_n.
 * https://oeis.org/A003764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6a(n-1) + 16a(n-2) - 29a(n-3) - 16a(n-4) + 16a(n-5), a(1)
 *            = 1, a(2) = 13, a(3) = 85, a(4) = 673, a(5) = 5021
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n) {
    if (n == 1) return 1;
    if (n == 2) return 13;
    if (n == 3) return 85;
    if (n == 4) return 673;
    if (n == 5) return 5021;
    
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache = new_cache;
        
        if (cache_len == 0) {
            cache[1] = 1;
            cache[2] = 13;
            cache[3] = 85;
            cache[4] = 673;
            cache[5] = 5021;
            cache_len = 6;
        }
        
        for (idx_t i = cache_len; i <= n; i++) {
            cache[i] = 6 * cache[i-1] + 16 * cache[i-2] - 29 * cache[i-3] - 16 * cache[i-4] + 16 * cache[i-5];
        }
        
        cache_len = new_len;
    }
    
    return cache[n];
}
