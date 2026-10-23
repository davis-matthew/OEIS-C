/*
 * A029035  Expansion of 1/((1-x)*(1-x^3)*(1-x^4)*(1-x^9)).
 * https://oeis.org/A029035
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 340, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Calculates a(n) = number of partitions of n into the parts 1,3,4,9.
   The result is computed with dynamic programming (coin‑change counting)
   and memoised between calls. */

#include <stdlib.h>  /* malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;   /* array of counts */
    static idx_t  size  = 0;      /* current number of entries stored */

    /* extend cache if needed */
    if (n >= size) {
        /* new size needs at most n+1 entries */
        idx_t newsize = n + 1;

        /* reallocate memory */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)newsize * sizeof(seq_t));
        if (tmp == NULL)
            return 0;           /* out of memory → return 0 as a safe guard */

        cache = tmp;

        /* initialise the new entries (set to 0) */
        for (idx_t i = size; i < newsize; ++i)
            cache[i] = 0;

        /* base case: a(0) = 1 */
        cache[0] = 1;

        /* extra step: after enlarging, rebuild all entries up to n
           from scratch – DP with the parts {1,3,4,9}. */
        const idx_t parts[4] = {1, 3, 4, 9};

        /* reset values to 0 except a(0) which is already 1 */
        for (idx_t i = 1; i <= n; ++i)
            cache[i] = 0;

        for (int p = 0; p < 4; ++p) {
            idx_t part = parts[p];
            for (idx_t i = 0; i + part <= n; ++i)
                cache[i + part] += cache[i];
        }

        /* update current size */
        size = newsize;
    }

    /* return memoised value */
    return cache[n];
}
