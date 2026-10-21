/*
 * A210383  Number of (n+1) X 2 0..2 arrays with every 2 X 2 subblock having
 *          one, two or three distinct clockwise edge differences.
 * https://oeis.org/A210383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 9*a(n-1) - 6*a(n-2) - 6*a(n-3)   (n ≥ 4)
 * Initial values: a(1) = 73,   a(2) = 597,   a(3) = 4881
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) // sequence is defined for n ≥ 1 only
        return (seq_t)0;

    /* static memoisation cache */
    static seq_t *cache      = NULL; /* array of seq_t, 1‑based */
    static size_t cache_cap  = 0;   /* allocated size */
    static idx_t  cache_size = 0;   /* highest computed index */

    /* initialise cache with the first three terms */
    if (cache == NULL) {
        /* allocate space for indices 0..3 (index 0 unused) */
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* allocation failure – unlikely */
        cache[0] = (seq_t)0;                /* unused */
        cache[1] = (seq_t)73;
        cache[2] = (seq_t)597;
        cache[3] = (seq_t)4881;
        cache_size = 3;
    }

    /* return immediately if the value is already computed */
    if (n <= cache_size)
        return cache[(size_t)n];

    /* grow the cache to hold index n (size must be ≥ n+1) */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0; /* allocation failure – unlikely */
        cache     = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms using the recurrence */
    for (idx_t k = cache_size + 1; k <= n; ++k) {
        /* all operations are performed in seq_t, no overflow checks needed
           because seq_t may be a wide/unbounded integer type. */
        cache[(size_t)k] = (seq_t)9 * cache[(size_t)(k-1)]
                         - (seq_t)6 * cache[(size_t)(k-2)]
                         - (seq_t)6 * cache[(size_t)(k-3)];
    }
    cache_size = n;
    return cache[(size_t)n];
}
