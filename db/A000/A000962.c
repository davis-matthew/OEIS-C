/*
 * A000962  The convergent sequence A_n for the ternary continued fraction
 *          (3,1;2,2) of period 2.
 * https://oeis.org/A000962
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-2) - 3*a(n-4) + a(n-6) for n >= 6
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 26, u64 n <= 50, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t max_computed = 5;

seq_t a(idx_t n) {
    if (n < 6) {
        static const seq_t base[] = {1, 0, 0, 1, 2, 5};
        return base[n];
    }
    
    // Initialize cache on first call
    if (cache == NULL) {
        cache_size = (n < 100) ? 100 : (n + 10);
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 1;
        cache[4] = 2;
        cache[5] = 5;
    }
    
    // Expand cache if needed
    if (n >= cache_size) {
        seq_t *new_cache = (seq_t *)malloc((n + 10) * sizeof(seq_t));
        for (idx_t i = 0; i < cache_size; i++) {
            new_cache[i] = cache[i];
        }
        free(cache);
        cache = new_cache;
        cache_size = n + 10;
    }
    
    // Compute any missing values up to n
    for (idx_t i = max_computed + 1; i <= n; i++) {
        cache[i] = 7 * cache[i-2] - 3 * cache[i-4] + cache[i-6];
    }
    max_computed = (n > max_computed) ? n : max_computed;
    
    return cache[n];
}
