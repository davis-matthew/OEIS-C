/*
 * A217978  Number of n X 4 arrays of the minimum value of corresponding
 *          elements and their horizontal, vertical, diagonal or antidiagonal
 *          neighbors in a random 0..1 n X 4 array.
 * https://oeis.org/A217978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 5*a(n-2) + a(n-3) - 5*a(n-4) - 8*a(n-5) +
 *            a(n-6) + 2*a(n-7) Initial values (n = 1..7): a(1) = 6, a(2) = 6,
 *            a(3) = 36, a(4) = 98, a(5) = 362, a(6) = 1180, a(7) = 4046
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 18, u64 n <= 36, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* For n <= 7 we return the stored initial values. */
    static const seq_t init[7] = {6, 6, 36, 98, 362, 1180, 4046};

    if (n <= 7 && n >= 1)
        return init[n - 1];

    /* Static cache to memoize computed values. */
    static seq_t *cache = NULL;   /* cache[i] holds a(i+1) */
    static size_t cache_size = 0; /* number of values stored */

    /* Allocate initial cache and fill it with the first 7 values. */
    if (cache == NULL)
    {
        cache = (seq_t *)malloc(7 * sizeof(seq_t));
        if (!cache)
            return 0; /* allocation failure – should not happen for the test cases */
        for (size_t i = 0; i < 7; ++i)
            cache[i] = init[i];
        cache_size = 7;
    }

    /* Extend the cache up to the requested index n. */
    if (cache_size < (size_t)n)
    {
        size_t new_size = (size_t)n;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return 0; /* allocation failure */
        cache = tmp;
        /* Compute new terms using the recurrence. */
        for (size_t i = cache_size; i < new_size; ++i)
        {
            /* i corresponds to a(i+1) */
            /* Positive part of the recurrence */
            seq_t pos = 2 * cache[i - 1]                      /* 2*a(n-1) */
                      + 5 * cache[i - 2]                      /* 5*a(n-2) */
                      + cache[i - 3]                          /* a(n-3) */
                      + cache[i - 6]                          /* a(n-6) */
                      + 2 * cache[i - 7];                     /* 2*a(n-7) */

            /* Negative part of the recurrence */
            seq_t neg = 5 * cache[i - 4] + 8 * cache[i - 5];

            cache[i] = pos - neg;   /* pos >= neg for all n >= 8 */
        }
        cache_size = new_size;
    }

    return cache[n - 1];
}
