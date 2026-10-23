/*
 * A144783  Variant of Sylvester's sequence: a(n+1) = a(n)^2 - a(n) + 1, with
 *          a(1) = 10.
 * https://oeis.org/A144783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Variant of Sylvester's sequence:
 * a(n+1) = a(n)^2 - a(n) + 1,
 * a(1) = 10
 * (OEIS A144783)
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 */
    static seq_t *cache = NULL;   /* 0‑based cache: cache[0] = a(1) */
    static size_t cache_size = 0; /* number of terms currently cached */

    if (n == 0)
        return (seq_t)0; /* undefined for offset 1, safe fallback */

    /* Extend cache if necessary */
    if (cache_size < (size_t)n)
    {
        /* Allocate or reallocate memory for the cache */
        if (cache_size == 0)
        {
            cache = (seq_t *)malloc(sizeof(seq_t) * (size_t)n);
            if (!cache)
                return (seq_t)0;  /* allocation failure, undefined result */
            cache[0] = (seq_t)10; /* a(1) */
            cache_size = 1;
        }
        else
        {
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * (size_t)n);
            if (!tmp)
                return (seq_t)0; /* allocation failure */
            cache = tmp;
        }

        /* Compute missing terms */
        for (size_t i = cache_size; i < (size_t)n; ++i)
        {
            seq_t prev = cache[i - 1];
            cache[i] = prev * prev - prev + (seq_t)1;
        }
        cache_size = (size_t)n;
    }

    return cache[n - 1];
}
