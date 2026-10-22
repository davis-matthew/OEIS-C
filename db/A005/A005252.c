/*
 * A005252  a(n) = Sum_{k=0..floor(n/4)} binomial(n-2k,2k).
 * https://oeis.org/A005252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-4); a(0) = a(1) = a(2) = a(3) = 1
 * Range:     u8 n <= 13, u16 n <= 25, u32 n <= 48, u64 n <= 94, u128 n <= 186,
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
    static idx_t max_computed = 3;
    static bool initialized = false;
    
    if (!initialized) {
        initialized = true;
        cache_len = 100;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = cache[1] = cache[2] = cache[3] = 1;
    }
    
    if (n <= 3) {
        return 1;
    }
    
    if (cache_len <= n) {
        idx_t new_len = (n + 1) * 2;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        
        for (idx_t i = 0; i < cache_len; i++) {
            new_cache[i] = cache[i];
        }
        free(cache);
        
        cache = new_cache;
        cache_len = new_len;
    }
    
    for (idx_t i = max_computed + 1; i <= n; i++) {
        cache[i] = 2 * cache[i-1] - cache[i-2] + cache[i-4];
    }
    if (n > max_computed) {
        max_computed = n;
    }
    
    return cache[n];
}
