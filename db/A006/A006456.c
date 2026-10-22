/*
 * A006456  Number of compositions (ordered partitions) of n into squares.
 * https://oeis.org/A006456
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(0) = 1; a(n) = Sum_{1 <= k^2 <= n} a(n-k^2), if n > 0
 * Range:     u8 n <= 18, u16 n <= 33, u32 n <= 65, u64 n <= 129,
 *            u128 n <= 256, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        
        if (cache) {
            for (idx_t i = 0; i < cache_len; i++) {
                new_cache[i] = cache[i];
            }
            free(cache);
        }
        cache = new_cache;
        
        if (cache_len == 0) {
            cache[0] = 1;
            cache_len = 1;
        }
        
        for (idx_t i = cache_len; i <= n; i++) {
            seq_t sum = 0;
            for (idx_t k = 1; k * k <= i; k++) {
                sum += cache[i - k * k];
            }
            cache[i] = sum;
        }
        
        cache_len = new_len;
    }
    
    return cache[n];
}
