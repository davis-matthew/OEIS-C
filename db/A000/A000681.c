/*
 * A000681  Number of n X n matrices with nonnegative entries and every row
 *          and column sum 2.
 * https://oeis.org/A000681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = n^2*a(n-1) - (1/2)*n*(n-1)^2*a(n-2), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n == 0 || n == 1) return 1;
    
    static idx_t cached_n = 1;
    static seq_t *cache = NULL;
    static idx_t cache_capacity = 0;
    
    if (cache == NULL) {
        cache_capacity = 16;
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 1;
        cached_n = 1;
    }
    
    // Expand cache if needed
    while (n >= cache_capacity) {
        idx_t new_capacity = cache_capacity * 2;
        seq_t *new_cache = (seq_t *)malloc(new_capacity * sizeof(seq_t));
        memcpy(new_cache, cache, (cached_n + 1) * sizeof(seq_t));
        free(cache);
        cache = new_cache;
        cache_capacity = new_capacity;
    }
    
    // Compute missing values
    for (idx_t i = cached_n + 1; i <= n; i++) {
        seq_t ni = (seq_t)i;
        seq_t ni_minus_1 = (seq_t)(i - 1);
        seq_t term1 = ni * ni * cache[i - 1];
        
        seq_t term2_coeff;
        if (i & 1) {  // i is odd
            term2_coeff = ni * ((ni_minus_1 >> 1) * ni_minus_1);
        } else {  // i is even
            term2_coeff = (ni >> 1) * (ni_minus_1 * ni_minus_1);
        }
        
        seq_t term2 = term2_coeff * cache[i - 2];
        cache[i] = term1 - term2;
    }
    
    cached_n = n;
    return cache[n];
}
