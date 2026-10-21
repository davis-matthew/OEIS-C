/*
 * A205579  a(n) = round(r^n) where r is the smallest Pisot number (real root
 *          r=1.3247179.. of x^3-x-1).
 * https://oeis.org/A205579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2) + a(n-3) for n >= 13 a(0) = 1, a(1) = 1, a(2) = 2,
 *            a(3) = 2, a(4) = 3, a(5) = 4, a(6) = 5, a(7) = 7, a(8) = 9, a(9)
 *            = 13, a(10) = 17, a(11) = 22, a(12) = 29
 * Range:     u8 n <= 19, u16 n <= 39, u32 n <= 78, u64 n <= 157,
 *            u128 n <= 315, big any n
 * Helpers:   ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>           /* for size_t */
#include "../utils/ensure_capacity.h"   /* for ensure_capacity */

seq_t a(idx_t n)
{
    /* Initial values up to n = 12 */
    static const seq_t init[13] = {1, 1, 2, 2, 3, 4, 5, 7, 9, 13, 17, 22, 29};

    /* Memoization cache */
    static seq_t *cache = NULL;   /* array of computed terms */
    static size_t cache_cap = 0;  /* number of entries currently in cache */

    /* Convert n to size_t for indexing (n is expected to be < 2^64) */
    size_t up_to = (size_t)n;

    /* Remember how many entries were already computed */
    size_t last_filled = cache_cap;

    /* Ensure the cache has room for indices 0 .. up_to */
    ensure_capacity(up_to + 1, &cache_cap, (void **)&cache, sizeof(seq_t));

    /* Fill any missing values up to the requested index */
    for (size_t i = last_filled; i <= up_to; ++i) {
        if (i < 13) {
            cache[i] = init[i];
        } else {
            cache[i] = cache[i - 2] + cache[i - 3];
        }
    }

    /* Update the recorded capacity to the actual number of filled entries */
    cache_cap = up_to + 1;

    return cache[up_to];
}
