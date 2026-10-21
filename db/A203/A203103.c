/*
 * A203103  Number of n X 3 binary arrays with every 1 immediately preceded by
 *          0 0 to the left or above.
 * https://oeis.org/A203103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A203103
 * a(1) = 2, a(2) = 4, a(3) = 23, a(4) = 90, a(5) = 296,
 * a(6) = 1125, a(7) = 4314, a(8) = 15844, a(9) = 58745
 *
 * Formula (9‑term linear recurrence):
 *   a(n) =  2*a(n-1) + 2*a(n-2) + 15*a(n-3) + 8*a(n-4)
 *         - 4*a(n-5) -17*a(n-6) -12*a(n-7) -10*a(n-8) + 5*a(n-9)
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return 0 for the undefined index 0 */
    if (n == 0)
        return 0;

    /* Static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;   /* number of allocated slots */
    static idx_t   cache_max = 0;   /* highest index stored in cache */

    /* Hard‑coded first nine terms (1‑based).  cache[0] is unused. */
    static const seq_t init[10] = {
        0,          /* dummy 0 for 1‑based indexing */
        2, 4, 23, 90, 296, 1125, 4314, 15844, 58745
    };

    /* Initialise the cache on the first call */
    if (cache == NULL) {
        cache_cap = 10;                 /* slots 0 … 9 */
        cache     = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache)            /* allocation failure – return 0 */
            return 0;

        /* copy the known terms */
        for (idx_t i = 1; i <= 9; ++i)
            cache[i] = init[i];
        cache_max = 9;
    }

    /* If the desired term is already cached, return it */
    if (n <= cache_max)
        return cache[n];

    /* Ensure the cache has room for the new index */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;           /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)          /* reallocation failure – return 0 */
            return 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute all missing terms up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        seq_t term;
        if (i <= 9) {             /* safety check, should not happen */
            term = init[i];
        } else {
            seq_t a1 = cache[i-1];
            seq_t a2 = cache[i-2];
            seq_t a3 = cache[i-3];
            seq_t a4 = cache[i-4];
            seq_t a5 = cache[i-5];
            seq_t a6 = cache[i-6];
            seq_t a7 = cache[i-7];
            seq_t a8 = cache[i-8];
            seq_t a9 = cache[i-9];

            /* Positive contributions */
            term = (a1 << 1) + (a2 << 1)
                 + (a3 << 3) + (a3 << 2) + (a3 << 1) + a3    /* 15*a3 */
                 + (a4 << 3);                                /*  8*a4 */

            /* Negative contributions */
            term -= (a5 << 2);                                 /*  4*a5 */
            term -= (a6 << 4) + a6;                            /* 17*a6 */
            term -= (a7 << 3) + (a7 << 2);                    /* 12*a7 */
            term -= (a8 << 3) + (a8 << 1);                    /* 10*a8 */

            /* Remaining positive term */
            term += (a9 << 2) + a9;                            /*  5*a9 */
        }
        cache[i] = term;
    }

    cache_max = n;
    return cache[n];
}
