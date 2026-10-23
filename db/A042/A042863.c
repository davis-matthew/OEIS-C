/*
 * A042863  Denominators of continued fraction convergents to sqrt(963).
 * https://oeis.org/A042863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = 1924*a(n-2) - a(n-4) for n>3 initial terms: a(0)=1,
 *            a(1)=31, a(2)=1923, a(3)=59644
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* cached sequence values */
    static size_t cache_cap  = 0;      /* allocated capacity of cache   */
    static size_t cache_len  = 0;      /* number of terms currently cached */

    /* Constant initial terms for indices 0…3 */
    static const seq_t base[4] = { (seq_t)1,
                                   (seq_t)31,
                                   (seq_t)1923,
                                   (seq_t)59644 };

    /* Return value directly for the first four indices. */
    if (n < 4u) {
        return base[(size_t)n];
    }

    /* Ensure the cache can hold at least (n+1) terms. */
    size_t required = (size_t)n + 1;
    if (required > cache_cap) {
        size_t new_cap = required;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed: return a safe default (zero). */
            return (seq_t)0;
        }
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the cache with the base values if this is the first call. */
    if (cache_len == 0) {
        for (size_t i = 0; i < 4; ++i) {
            cache[i] = base[i];
        }
        cache_len = 4;
    }

    /* Compute all missing terms up to and including n. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        cache[i] = cache[i - 2] * (seq_t)1924 - cache[i - 4];
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
