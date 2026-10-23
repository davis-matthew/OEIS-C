/*
 * A191501  Expansion of 2-sqrt(1-4*x-4*x^2-4*x^3).
 * https://oeis.org/A191501
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((4*n-6)*a(n-1)+(4*n-12)*a(n-2)+(4*n-18)*a(n-3))/n for
 *            n≥5 Base values: a(0)=1, a(1)=2, a(2)=4, a(3)=10, a(4)=28
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    /* Ensure cache contains at least n+1 terms */
    if (n + 1 > cache_size)
    {
        /* If cache is empty, allocate with space for base values */
        if (cache_size == 0)
        {
            cache = (seq_t *)malloc(5 * sizeof(seq_t));
            if (!cache)
                exit(EXIT_FAILURE);  /* memory allocation failure */
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)2;
            cache[2] = (seq_t)4;
            cache[3] = (seq_t)10;
            cache[4] = (seq_t)28;
            cache_size = 5;
        }

        /* Grow the cache to the requested size */
        if (n + 1 > cache_size)
        {
            seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
            if (!tmp)
                exit(EXIT_FAILURE);
            cache = tmp;
            /* Compute new terms using the recurrence */
            for (idx_t i = cache_size; i <= n; ++i)
            {
                /* Coefficients (all non‑negative for i≥5) */
                seq_t c1 = (seq_t)(4 * i - 6);
                seq_t c2 = (seq_t)(4 * i - 12);
                seq_t c3 = (seq_t)(4 * i - 18);

                /* Numerator: c1*a(i-1) + c2*a(i-2) + c3*a(i-3) */
                seq_t num = c1 * cache[i - 1] + c2 * cache[i - 2] + c3 * cache[i - 3];

                /* a(i) = num / i */
                cache[i] = num / (seq_t)i;
            }
            cache_size = n + 1;
        }
    }

    return cache[n];
}
