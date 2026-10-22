/*
 * A006452  a(n) = 6*a(n-2) - a(n-4).
 * https://oeis.org/A006452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-2) - a(n-4), a(0) = 1, a(1) = 1, a(2) = 2, a(3) = 4
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 26, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    
    if (n == 0 || n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 4;
    
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        
        if (cache_size == 0) {
            cache[0] = 1;
            cache[1] = 1;
            cache[2] = 2;
            cache[3] = 4;
            
            for (idx_t i = 4; i < new_size; i++) {
                cache[i] = 6 * cache[i-2] - cache[i-4];
            }
        } else {
            for (idx_t i = cache_size; i < new_size; i++) {
                cache[i] = 6 * cache[i-2] - cache[i-4];
            }
        }
        
        cache_size = new_size;
    }
    
    return cache[n];
}
