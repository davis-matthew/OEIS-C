/*
 * A003729  Number of perfect matchings (or domino tilings) in graph C_{5} X
 *          P_{2n}.
 * https://oeis.org/A003729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19*a(n-1) - 41*a(n-2) + 19*a(n-3) - a(n-4), with a(1) =
 *            11, a(2) = 176, a(3) = 2911, a(4) = 48301
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n == 1) return 11;
    if (n == 2) return 176;
    if (n == 3) return 2911;
    if (n == 4) return 48301;
    
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t max_computed = 4;
    
    if (n <= max_computed && cache != NULL) {
        return cache[n - 1];
    }
    
    if (cache == NULL) {
        cache_size = (n > 100) ? n + 100 : 100;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = 11;
        cache[1] = 176;
        cache[2] = 2911;
        cache[3] = 48301;
        max_computed = 4;
    } else if (n > cache_size) {
        cache_size = n + 100;
        seq_t *new_cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        memcpy(new_cache, cache, max_computed * sizeof(seq_t));
        free(cache);
        cache = new_cache;
    }
    
    for (idx_t i = max_computed + 1; i <= n; i++) {
        seq_t val = 19 * cache[i - 2] - 41 * cache[i - 3] + 19 * cache[i - 4] - cache[i - 5];
        cache[i - 1] = val;
    }
    
    max_computed = n;
    return cache[n - 1];
}
