/*
 * A185513  Number of (n+1) X 2 0..2 arrays with every 2 X 2 subblock
 *          singular.
 * https://oeis.org/A185513
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A185513 :  Number of (n+1) × 2 0..2 arrays with every 2 × 2 subblock singular.
 *  Empirical recurrence:
 *      a(1) = 31, a(2) = 143, a(3) = 593,
 *      a(n) = 4*a(n-1) + 3*a(n-2) - 8*a(n-3)   (n ≥ 4)
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset */
    if (n < 1)
        return (seq_t)0;

    /* Static cache: 1‑based, extended dynamically on demand */
    static seq_t *cache = NULL;      /* cache[0] unused */
    static size_t cache_cap = 0;     /* allocated size */
    static idx_t max_cached = 0;     /* highest computed index */

    /* Ensure the cache is large enough for index n */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache)          /* allocation failure */
            return (seq_t)0;
        cache_cap = new_cap;
    }

    /* Initialise the first three terms once */
    if (max_cached == 0) {
        /* make sure we have space for indices 0..3 */
        if (cache_cap < 4) {
            size_t new_cap = 4;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!cache)
                return (seq_t)0;
            cache_cap = new_cap;
        }
        cache[0] = (seq_t)0;          /* dummy */
        cache[1] = (seq_t)31;         /* a(1) */
        cache[2] = (seq_t)143;        /* a(2) */
        cache[3] = (seq_t)593;        /* a(3) */
        max_cached = 3;
    }

    /* If the value is already computed, return it */
    if (n <= max_cached)
        return cache[n];

    /* Compute all missing terms up to n using the recurrence */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t term = cache[i-1] * (seq_t)4;
        term += cache[i-2] * (seq_t)3;
        term -= cache[i-3] * (seq_t)8;
        cache[i] = term;
    }

    max_cached = n;
    return cache[n];
}
