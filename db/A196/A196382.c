/*
 * A196382  Number of sequences of n coin flips, that win on the last flip, if
 *          the sequence of flips ends with (1,1,0) or (1,0,1).
 * https://oeis.org/A196382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) + 1 for n>3 initial values: a(1)=0,
 *            a(2)=0, a(3)=2
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 58, u64 n <= 116,
 *            u128 n <= 232, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* external linkage function for the sequence */
seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;

    /* initialise cache with known values */
    if (cache == NULL) {
        /* allocate space for indices 0..3 (index 0 unused), using size 4 */
        cache_size = 4;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL)
            abort();                 /* cannot continue without memory */

        /* set initial values, index 0 is unused but set to 0 for safety */
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 0;
        cache[3] = 2;
    }

    /* expand cache if needed for the requested n */
    if ((size_t)n >= cache_size) {
        /* allocate space up to n (inclusive) */
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL)
            abort();                  /* memory failure */
        cache = tmp;
        /* compute missing terms using the recurrence */
        for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
            /* for i==1,2,3 cache already set */
            if (i > 3)
                cache[i] = cache[i - 1] + cache[i - 3] + 1;
        }
        cache_size = new_size;
    }

    return cache[(size_t)n];
}
