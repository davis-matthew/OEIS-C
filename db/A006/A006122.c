/*
 * A006122  Sum of Gaussian binomial coefficients [ n,k ] for q=8.
 * https://oeis.org/A006122
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1)+(8^(n-1)-1)*a(n-2), a(0) = 1, a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t computed_len = 0;

seq_t a(idx_t n) {
    // Ensure cache is large enough
    if (cache == NULL || n >= cache_len) {
        idx_t new_len = (n < 16) ? 16 : (n + 1);
        seq_t *new_cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (new_cache == NULL) return 0;
        
        if (cache != NULL) {
            memcpy(new_cache, cache, cache_len * sizeof(seq_t));
            free(cache);
        } else {
            memset(new_cache, 0, new_len * sizeof(seq_t));
        }
        
        cache = new_cache;
        cache_len = new_len;
    }
    
    // Initialize base cases
    if (computed_len == 0) {
        cache[0] = (seq_t)1;
        if (cache_len > 1) cache[1] = (seq_t)2;
        computed_len = 2;
    }
    
    // Compute values from computed_len to n
    if (n >= computed_len) {
        // Compute pow8 = 8^(computed_len-1)
        seq_t pow8 = (seq_t)1;
        for (idx_t j = 0; j < computed_len - 1; j++) {
            pow8 = pow8 * (seq_t)8;
        }
        
        for (idx_t i = computed_len; i <= n; i++) {
            cache[i] = (seq_t)2 * cache[i-1] + (pow8 - (seq_t)1) * cache[i-2];
            pow8 = pow8 * (seq_t)8;
        }
        
        computed_len = n + 1;
    }
    
    return cache[n];
}
