/*
 * A003692  Number of trees on n labeled vertices with degree at most 3.
 * https://oeis.org/A003692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   (n+3)*a(n) = (n+1)*(2*n+1)*a(n-1) + (n-2)*n*(n+1)*a(n-2), a(0) =
 *            1, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    
    if (cache == NULL) {
        cache_len = 2;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 1;
    }
    
    if (n >= cache_len) {
        idx_t old_len = cache_len;
        idx_t new_len = cache_len;
        while (new_len <= n) {
            new_len *= 2;
        }
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        for (idx_t i = old_len; i < new_len; i++) {
            cache[i] = 0;
        }
        cache_len = new_len;
    }
    
    for (idx_t i = 2; i <= n; i++) {
        if (cache[i] == 0) {
            seq_t coeff1 = (seq_t)(i + 1) * (seq_t)(2 * i + 1);
            seq_t term1 = coeff1 * cache[i - 1];
            
            seq_t coeff2 = (seq_t)(i - 2) * (seq_t)i * (seq_t)(i + 1);
            seq_t term2 = coeff2 * cache[i - 2];
            
            seq_t numerator = term1 + term2;
            seq_t denominator = (seq_t)(i + 3);
            
            cache[i] = numerator / denominator;
        }
    }
    
    return cache[n];
}
