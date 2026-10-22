/*
 * A001333  Pell-Lucas numbers: numerators of continued fraction convergents
 *          to sqrt(2).
 * https://oeis.org/A001333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, core, nice, frac
 * Formula:   a(n) = 2*a(n-1) + a(n-2), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 51, u128 n <= 101,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (cache == NULL) {
        // Initialize on first call
        idx_t init_size = (n < 100) ? 100 : (n + 10);
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 1;
        cache_len = 2;
    }
    
    // Ensure we have computed up to n
    if (cache_len <= n) {
        // Reallocate if needed
        idx_t new_capacity = (n + 1) * 2;
        cache = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        
        // Compute missing values
        for (idx_t i = cache_len; i <= n; i++) {
            cache[i] = 2 * cache[i-1] + cache[i-2];
        }
        cache_len = n + 1;
    }
    
    return cache[n];
}
