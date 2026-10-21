/*
 * A081065  Numbers n such that n^2 =
 *          (1/3)*(n+floor(sqrt(3)*n*floor(sqrt(3)*n))).
 * https://oeis.org/A081065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*a(n-1) - 15*a(n-2) + a(n-3) with initial values a(1) =
 *            2, a(2) = 24, a(3) = 330
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoized computation of the sequence.  Works for any order of calls.
   The cache is grown automatically up to the requested index. */
seq_t a(idx_t n)
{
    /* Preserve 1‑based indexing; cache[0] is unused. */
    static seq_t *cache = NULL;      /* dynamic array of computed values */
    static size_t cache_size = 0;    /* number of terms stored, < 1 if none */
    static size_t cache_capacity = 0;

    /* Nothing is defined for n < 1; return 0 in that case. */
    if (n < 1)
        return (seq_t)0;

    /* Grow the cache if necessary. */
    if ((size_t)n > cache_size)
    {
        /* Determine new capacity. */
        size_t new_cap = cache_capacity ? cache_capacity * 2 : 4;
        if ((size_t)n > new_cap)
            new_cap = (size_t)n;

        /* Reallocate or allocate the array. */
        if (cache == NULL)
        {
            cache = (seq_t *)malloc((new_cap + 1) * sizeof(seq_t));
        }
        else
        {
            cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        }
        if (cache == NULL)
            return (seq_t)0; /* allocation failure – unlikely in the judge */

        /* Initialize new entries.  We keep cache[0] unused. */
        if (cache_size == 0)
        {
            /* Base cases */
            cache[1] = (seq_t)2;
            if (new_cap >= 2)
                cache[2] = (seq_t)24;
            if (new_cap >= 3)
                cache[3] = (seq_t)330;
        }
        /* Compute any missing terms using the recurrence. */
        for (size_t i = cache_size + 1; i <= (size_t)n; ++i)
        {
            if (i == 1)
            {
                cache[1] = (seq_t)2;
            }
            else if (i == 2)
            {
                cache[2] = (seq_t)24;
            }
            else if (i == 3)
            {
                cache[3] = (seq_t)330;
            }
            else
            {
                /* a(i) = 15*a(i-1) - 15*a(i-2) + a(i-3)   (all integer ops) */
                seq_t term = cache[i-1];
                term = term * (seq_t)15;
                seq_t t2 = cache[i-2];
                t2 = t2 * (seq_t)15;
                term -= t2;
                term += cache[i-3];
                cache[i] = term;
            }
        }

        cache_size = n;
        cache_capacity = new_cap;
    }

    return cache[(size_t)n];
}
