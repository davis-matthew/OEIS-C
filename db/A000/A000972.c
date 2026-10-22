/*
 * A000972  Fermat coefficients.
 * https://oeis.org/A000972
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in x^7*(1 + 6*x + 9*x^2 + 9*x^3 +
 *            10*x^4 + 7*x^5 + 12*x^6 + 6*x^7 + 4*x^8) / ((1 - x)^7*(1 + x +
 *            x^2 + x^3 + x^4 + x^5 + x^6))
 * Range:     u8 n <= 10, u16 n <= 17, u32 n <= 88, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t computed_up_to = 0;

seq_t a(idx_t n) {
    if (n < 7) return 0;
    
    if (cache == NULL) {
        cache_size = (n + 1 < 128) ? 128 : n + 128;
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        cache[7] = 1;
        computed_up_to = 7;
    }
    
    while (n >= cache_size) {
        idx_t new_size = cache_size * 2;
        seq_t *new_cache = (seq_t *)calloc(new_size, sizeof(seq_t));
        for (idx_t i = 0; i < cache_size; i++) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cache_size = new_size;
    }
    
    static const int num[] = {1, 6, 9, 9, 10, 7, 12, 6, 4};
    
    for (idx_t i = computed_up_to + 1; i <= n; i++) {
        seq_t sum = (seq_t)6 * cache[i - 1]
                  - (seq_t)15 * cache[i - 2]
                  + (seq_t)20 * cache[i - 3]
                  - (seq_t)15 * cache[i - 4]
                  + (seq_t)6 * cache[i - 5]
                  - (seq_t)1 * cache[i - 6]
                  + (seq_t)1 * cache[i - 7];
        
        if (i >= 8) sum -= (seq_t)6 * cache[i - 8];
        if (i >= 9) sum += (seq_t)15 * cache[i - 9];
        if (i >= 10) sum -= (seq_t)20 * cache[i - 10];
        if (i >= 11) sum += (seq_t)15 * cache[i - 11];
        if (i >= 12) sum -= (seq_t)6 * cache[i - 12];
        if (i >= 13) sum += (seq_t)1 * cache[i - 13];
        
        if (i <= 15) {
            sum += (seq_t)num[i - 7];
        }
        
        cache[i] = sum;
    }
    computed_up_to = n;
    
    return cache[n];
}
