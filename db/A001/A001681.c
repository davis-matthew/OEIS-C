/*
 * A001681  The partition function G(n,4).
 * https://oeis.org/A001681
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   6*a(n) = 6*a(n-1) + 6*(n-1)*a(n-2) + 3*(n-2)*(n-1)*a(n-3) +
 *            (n-3)*(n-2)*(n-1)*a(n-4), a(0) = 1, a(1) = 1, a(2) = 2, a(3) = 5
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 15, u64 n <= 26, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 1;
    if (n == 2) return 2;
    if (n == 3) return 5;
    
    // Ensure cache is large enough
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
        
        if (cache != NULL) {
            for (idx_t i = 0; i < cache_size; i++) {
                new_cache[i] = cache[i];
            }
            free(cache);
        } else {
            new_cache[0] = 1;
            new_cache[1] = 1;
            new_cache[2] = 2;
            new_cache[3] = 5;
        }
        
        cache = new_cache;
        idx_t old_size = cache_size;
        cache_size = new_size;
        
        idx_t start = (old_size == 0) ? 4 : old_size;
        for (idx_t i = start; i <= n; i++) {
            seq_t a_i_1 = cache[i - 1];
            seq_t a_i_2 = cache[i - 2];
            seq_t a_i_3 = cache[i - 3];
            seq_t a_i_4 = cache[i - 4];
            
            seq_t term1 = a_i_1;
            seq_t term2 = (seq_t)(i - 1) * a_i_2;
            seq_t term3 = ((seq_t)(i - 2) * (seq_t)(i - 1) / 2) * a_i_3;
            seq_t term4 = ((seq_t)(i - 3) * (seq_t)(i - 2) * (seq_t)(i - 1) / 6) * a_i_4;
            
            cache[i] = term1 + term2 + term3 + term4;
        }
    }
    
    return cache[n];
}
