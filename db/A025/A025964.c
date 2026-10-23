/*
 * A025964  Expansion of 1/((1-2*x)*(1-4*x)*(1-5*x)*(1-12*x)).
 * https://oeis.org/A025964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 23*a(n-1) - 170*a(n-2) + 496*a(n-3) - 480*a(n-4) a(0) =
 *            1, a(1) = 23, a(2) = 359, a(3) = 4843
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for computed values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;          /* allocated elements            */
    static idx_t   max_computed = (idx_t)-1;  /* highest index computed so far */

    /* 1. Initialise cache with the first four terms on first call */
    if (cache == NULL) {
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;   /* allocation failed – unlikely for n ≤ 10000 */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)23;
        cache[2] = (seq_t)359;
        cache[3] = (seq_t)4843;
        max_computed = 3;
    }

    /* 2. If the requested index is already known, return it */
    if (n <= max_computed)
        return cache[(size_t)n];

    /* 3. Ensure sufficient storage */
    if (cache_len <= (size_t)n) {
        size_t new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;   /* reallocation failed */
        cache = tmp;
        cache_len = new_len;
    }

    /* 4. Compute missing terms using the recurrence */
    for (size_t i = (size_t)max_computed + 1; i <= (size_t)n; ++i) {
        seq_t term = (seq_t)23 * cache[i - 1]
                   - (seq_t)170 * cache[i - 2]
                   + (seq_t)496 * cache[i - 3]
                   - (seq_t)480 * cache[i - 4];
        cache[i] = term;
    }

    /* 5. Update the highest computed index */
    max_computed = n;

    /* 6. Return the requested value */
    return cache[(size_t)n];
}
