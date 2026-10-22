/*
 * A006117  Sum of Gaussian binomial coefficients [ n,k ] for q=3.
 * https://oeis.org/A006117
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + (3^(n-1) - 1)*a(n-2), a(0) = 1, a(1) = 2
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static seq_t pow3(idx_t e) {
    seq_t result = 1;
    seq_t base = 3;
    while (e > 0) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n) {
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 2);
        cache[0] = 1;
        cache[1] = 2;
        cache_len = 2;
    }
    
    if (n < cache_len) {
        return cache[n];
    }
    
    cache = (seq_t *)realloc(cache, sizeof(seq_t) * (n + 1));
    
    seq_t power_of_3 = pow3(cache_len - 1);
    
    for (idx_t i = cache_len; i <= n; i++) {
        cache[i] = 2 * cache[i - 1] + (power_of_3 - 1) * cache[i - 2];
        power_of_3 *= 3;
    }
    
    cache_len = n + 1;
    return cache[n];
}
