/*
 * A006453  Expansion of 1/sqrt(1 - 12x + x^2).
 * https://oeis.org/A006453
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence a(n) + 6*(-2*n+1)*a(n-1) + (n-1)^2*a(n-2) = 0,
 *            rearranged as a(n) = 6*(2*n-1)*a(n-1) - (n-1)^2*a(n-2), with
 *            a(0) = 1, a(1) = 6
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 13,
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
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 6;
        cache_len = 2;
    }
    
    if (n < cache_len) {
        return cache[n];
    }
    
    cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    
    for (idx_t i = cache_len; i <= n; i++) {
        seq_t n_seq = (seq_t)i;
        seq_t coeff1 = 6 * (2 * n_seq - 1);
        seq_t term1 = coeff1 * cache[i - 1];
        seq_t coeff2 = (n_seq - 1) * (n_seq - 1);
        seq_t term2 = coeff2 * cache[i - 2];
        cache[i] = term1 - term2;
    }
    
    cache_len = n + 1;
    return cache[n];
}
