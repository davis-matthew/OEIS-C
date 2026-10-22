/*
 * A001515  Bessel polynomial y_n(x) evaluated at x=1.
 * https://oeis.org/A001515
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(0) = 1, a(1) = 2; thereafter a(n) = (2*n-1)*a(n-1) + a(n-2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 2;
    
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t highest_computed = 1;
    
    if (cache == NULL) {
        cache_size = (n + 1) * 2;
        if (cache_size < 16) cache_size = 16;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = 1;
        cache[1] = 2;
    }
    
    if (n >= cache_size) {
        idx_t new_size = (n + 1) * 2;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (cache == NULL) abort();
        cache_size = new_size;
    }
    
    for (idx_t k = highest_computed + 1; k <= n; k++) {
        seq_t coeff = (seq_t)k;
        coeff = coeff * 2 - 1;
        cache[k] = coeff * cache[k - 1] + cache[k - 2];
    }
    
    highest_computed = n;
    
    return cache[n];
}
