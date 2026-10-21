/*
 * A218031  G.f. A(x) satisfies A(x) = 1 + x / A(x^2).
 * https://oeis.org/A218031
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; for n ≥ 1, a(n) = RHS - Σ_{k=0}^{n-1} a(k)·a((n-k)/2)
 *            (for n-k even), where RHS = 1 if n = 1, RHS = a(n/2) if n is
 *            even, else 0. This recurrence follows from the generating
 *            function A(x) = 1 + x / A(x²) and the coefficient comparison
 * Range:     i8 n <= 54, i16 n <= 98, i32 n <= 194, i64 n <= 380,
 *            i128 n <= 758, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of already computed values.  */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;       /* allocated size of cache array   */
    static idx_t  computed = 0;         /* next index that needs to be computed */

    /* Grow the array if necessary.  */
    size_t needed = (size_t)(n + 1);
    if (needed > cache_size)
    {
        size_t new_size = cache_size ? cache_size << 1 : 8;
        while (new_size < needed)
            new_size <<= 1;
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache_size = new_size;
    }

    /* Compute any missing terms up to n.  */
    for (idx_t i = computed; i <= n; ++i)
    {
        if (i == 0)
        {
            cache[0] = (seq_t)1;
        }
        else
        {
            seq_t sum = (seq_t)0;
            for (idx_t j = 0; j < i; ++j)
            {
                if (((i - j) & (idx_t)1) == 0)
                {
                    idx_t half = (i - j) >> 1;
                    sum += cache[j] * cache[half];
                }
            }

            seq_t rhs = (seq_t)0;
            if (i == 1)
                rhs = (seq_t)1;
            else if ((i & (idx_t)1) == 0)   /* even i */
                rhs = cache[i >> 1];

            cache[i] = rhs - sum;
        }
    }

    computed = n + 1;
    return cache[n];
}
