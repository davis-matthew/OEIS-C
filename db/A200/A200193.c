/*
 * A200193  Number of -n..n arrays x(0..3) of 4 elements with zero sum,
 *          adjacent elements differing by more than one, and elements
 *          alternately increasing and decreasing.
 * https://oeis.org/A200193
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + 2*a(n-3) - 3*a(n-4) + 3*a(n-5) -
 *            a(n-6) with initial values: a(1)=2, a(2)=14, a(3)=48, a(4)=120,
 *            a(5)=242, a(6)=426
 * Range:     u8 n <= 5, u16 n <= 31, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache      = NULL;   /* array holding computed terms */
    static size_t cache_cap  = 0;      /* allocated size (excluding index 0) */
    static size_t cache_len  = 0;      /* largest index that has been computed */

    /* initialise cache with the first six terms if necessary */
    if (cache == NULL)
    {
        /* start with room for at least 64 terms */
        cache_cap = 64;
        if ((size_t)n > cache_cap)       /* ensure we can hold the requested n */
            cache_cap = (size_t)n;
        cache = (seq_t *)malloc(sizeof(seq_t) * (cache_cap + 1)); /* index 0 unused */
        if (cache == NULL) return (seq_t)0; /* allocation failure */

        /* set base values and leave index 0 unused */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)14;
        cache[3] = (seq_t)48;
        cache[4] = (seq_t)120;
        cache[5] = (seq_t)242;
        cache[6] = (seq_t)426;
        cache_len = 6;
    }

    /* grow cache if the requested n is larger than the allocated capacity */
    if ((size_t)n > cache_cap)
    {
        size_t new_cap = cache_cap;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (new_cap + 1));
        if (tmp == NULL) return (seq_t)0; /* realloc failure */
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* compute terms up to n if they have not been computed yet */
    if (n > cache_len)
    {
        for (idx_t k = cache_len + 1; k <= n; ++k)
        {
            if (k <= 6) continue;   /* already initialised */

            /* recurrence: 3a(k-1) - 3a(k-2) + 2a(k-3) - 3a(k-4) + 3a(k-5) - a(k-6) */
            seq_t sum_pos = 3 * cache[(size_t)(k - 1)] +
                            2 * cache[(size_t)(k - 3)] +
                            3 * cache[(size_t)(k - 5)];
            seq_t sum_neg = 3 * cache[(size_t)(k - 2)] +
                            3 * cache[(size_t)(k - 4)] +
                            cache[(size_t)(k - 6)];
            cache[(size_t)k] = sum_pos - sum_neg;
        }
        cache_len = (size_t)n;
    }

    return cache[(size_t)n];
}
