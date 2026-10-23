/*
 * A021380  Decimal expansion of 1/376.
 * https://oeis.org/A021380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   decimal digits of the reciprocal 1/376, computed by iterative
 *            long division. a(0) = 0, a(1) = 0, a(2) = 2, …
 * Range:     u8 n <= 1, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The function keeps a growing cache of the already computed digits.
    Long division is performed only for the next missing indices, so that
    the total work for any sequence of calls is linear in the largest n
    seen.  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state shared between calls */
    static seq_t *cache      = NULL;   /* array of computed digits      */
    static size_t cache_cap  = 0;      /* allocated capacity (in seq_t) */
    static idx_t  cache_size = 0;      /* number of digits stored        */
    static seq_t  remainder  = 0;      /* remainder of the last division */

    /* constants used in the division */
    const seq_t denom = (seq_t)376;    /* divisor                     */
    const seq_t ten   = (seq_t)10;     /* multiplier for next digit   */

    /* Initialisation: compute the first 24 decimal digits.
       These are the non‑zero digits that appear before the repeating
       period of length 40.  */
    if (cache_size == 0) {
        cache_cap   = 24;                       /* at least 24 slots */
        cache       = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!cache) return (seq_t)0;            /* allocation failure */

        seq_t r = (seq_t)1;                    /* initial remainder = 1 */
        for (idx_t i = 0; i < 24; ++i) {
            r *= ten;                          /* 10 · r */
            cache[i] = r / denom;              /* next digit */
            r %= denom;                        /* next remainder */
        }
        remainder = r;           /* remainder after digit 23 */
        cache_size = 24;         /* indices 0 … 23 are now stored */
    }

    /* If the requested index is already in the cache, return it. */
    if ((idx_t)n < cache_size) {
        return cache[(size_t)n];
    }

    /* Ensure that the cache has enough capacity for index n. */
    while (cache_cap <= (size_t)n) {
        cache_cap <<= 1;                         /* grow by factor 2 */
    }
    if (cache_cap > cache_size) {                /* reallocate if needed */
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;               /* allocation failure */
        cache = tmp;
    }

    /* Compute all missing digits by continuing the long division. */
    for (idx_t i = cache_size; i <= (idx_t)n; ++i) {
        remainder *= ten;                        /* 10 · last remainder */
        cache[(size_t)i] = remainder / denom;    /* next digit   */
        remainder %= denom;                      /* next remainder */
    }

    cache_size = (idx_t)n + 1;                   /* update size */

    return cache[(size_t)n];
}
