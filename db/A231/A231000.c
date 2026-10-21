/*
 * A231000  Number of years after which a date can fall on the same day of the
 *          week, in the Julian calendar.
 * https://oeis.org/A231000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(0) = 0, a(1) = 5, a(2) = 6, a(3) = 11, * a(4) = 17, a(5) =
 *            22, a(6) = 23, a(7) = 28, * a(n) = a(n-1) + a(n-7) - a(n-8) for
 *            n > 7
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Static cache shared by all calls */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

seq_t a(idx_t n)
{
    /* Initial eight terms of the sequence */
    static const seq_t init[8] = {0, 5, 6, 11, 17, 22, 23, 28};

    /* First call: allocate cache and copy the initial values */
    if (cache_cap == 0) {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (size_t i = 0; i < cache_cap; ++i)
            cache[i] = init[i];
    }

    /* For indices below 8, return pre‑computed values */
    if (n < 8)
        return cache[(size_t)n];

    /* Grow cache if necessary */
    size_t required = (size_t)n + 1;
    size_t old_cap = cache_cap;
    if (required > cache_cap) {
        cache = (seq_t *)realloc(cache, required * sizeof(seq_t));
        cache_cap = required;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t k = (idx_t)old_cap; k <= n; ++k) {
        seq_t val = cache[(size_t)(k - 1)] +
                    cache[(size_t)(k - 7)] -
                    cache[(size_t)(k - 8)];
        cache[(size_t)k] = val;
    }

    return cache[(size_t)n];
}
