/*
 * A005770  Number of convex polygons of length 2n on square lattice whose
 *          leftmost bottom vertex and rightmost top vertex have the same
 *          x-coordinate.
 * https://oeis.org/A005770
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) - 55*a(n-2) + 120*a(n-3) - 125*a(n-4) +
 *            54*a(n-5) - 8*a(n-6) for n > 10; a(5) = 1, a(6) = 9, a(7) = 55,
 *            a(8) = 286, a(9) = 1362, a(10) = 6143
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n < 5) return 0;
    
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t highest_computed = 0;
    
    // Initialize cache
    if (cache == NULL) {
        cache_len = 11;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        memset(cache, 0, cache_len * sizeof(seq_t));
        
        cache[5] = 1;
        cache[6] = 9;
        cache[7] = 55;
        cache[8] = 286;
        cache[9] = 1362;
        cache[10] = 6143;
        
        highest_computed = 10;
    }
    
    // Extend cache if needed
    if (cache_len <= n) {
        idx_t new_len = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        memset(new_cache + cache_len, 0, (new_len - cache_len) * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_len = new_len;
    }
    
    // Compute values iteratively from highest_computed + 1 to n
    for (idx_t i = highest_computed + 1; i <= n; i++) {
        cache[i] = (seq_t)12 * cache[i-1] 
                 - (seq_t)55 * cache[i-2] 
                 + (seq_t)120 * cache[i-3] 
                 - (seq_t)125 * cache[i-4] 
                 + (seq_t)54 * cache[i-5] 
                 - (seq_t)8 * cache[i-6];
    }
    
    if (n > highest_computed) {
        highest_computed = n;
    }
    
    return cache[n];
}
