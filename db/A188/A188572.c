/*
 * A188572  a(n) = coefficient of sqrt(3) in the expansion of (1 + sqrt(2) +
 *          sqrt(3))^n sequence.
 * https://oeis.org/A188572
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1)+4*a(n-2)-16*a(n-3)+8*a(n-4) with initial values
 *            a(0)=0, a(1)=1, a(2)=2, a(3)=12
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 32, u128 n <= 63,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>             /* size_t */
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static idx_t cache_size  = 0;   /* last filled index + 1 */
    static size_t cache_cap  = 0;   /* allocated capacity */

    /* Grow the cache if we need more space */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        if (i == 0) {
            cache[i] = (seq_t)0;
        } else if (i == 1) {
            cache[i] = (seq_t)1;
        } else if (i == 2) {
            cache[i] = (seq_t)2;
        } else if (i == 3) {
            cache[i] = (seq_t)12;
        } else {
            cache[i] = (seq_t)(4 * cache[i-1]
                               + 4 * cache[i-2]
                               - 16 * cache[i-3]
                               + 8 * cache[i-4]);
        }
    }
    cache_size = n + 1;

    return cache[n];
}
