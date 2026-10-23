/*
 * A010903  Pisot sequence E(3,13): a(n) = floor(a(n-1)^2/a(n-2) + 1/2).
 * https://oeis.org/A010903
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 3*a(n-2), a(0) = 3, a(1) = 13
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    /* Static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */

    /* Initialize cache with first two terms */
    if (cache_len == 0) {
        ensure_capacity(2, &cache_cap, (void **)&cache, sizeof(seq_t));
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)13;
        cache_len = 2;
    }

    /* Return already computed value */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Allocate space for the required terms */
    size_t target_len = (size_t)n + 1ULL;
    if (target_len > cache_cap)
        ensure_capacity(target_len, &cache_cap, (void **)&cache, sizeof(seq_t));

    /* Compute missing terms using the linear recurrence */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t prevprev = cache[i - 2];
        seq_t prev     = cache[i - 1];
        seq_t val =   (seq_t)5 * prev
                    - (seq_t)3 * prevprev;
        cache[i] = val;
    }
    cache_len = target_len;   /* update cached length */

    return cache[(size_t)n];
}
