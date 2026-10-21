/*
 * A234854  E.g.f. satisfies: A'(x) = 1 + 4*A(x) + A(x)^2, where A(0)=0.
 * https://oeis.org/A234854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   This function implements the recurrence derived from the
 *            differential equation A'(x)=1+4A(x)+A(x)^2 with A(0)=0 and
 *            offset 1: a(1) = 1 a(n+1) = 4·a(n) + Σ_{k=1}^{n-1} binom(n,
 *            k)·a(k)·a(n-k) (n ≥ 1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc, free */
#include <stddef.h>   /* for size_t */

/* The types seq_t (the term type) and idx_t (the index type) are defined
   elsewhere in the build.  The function prototype is also declared in a
   header; we provide only the body here. */

seq_t a(idx_t n)
{
    /* Return 0 for indices below the sequence offset.  */
    if (n < 1)
        return (seq_t)0;

    /* Static cache for memoization; cache[1..max] holds a(1)...a(max).  */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* largest n for which cache is filled */

    /* Resize the cache if we haven't computed up to n yet.  */
    if (n > cache_size)
    {
        /* Allocate space for indices 0..n (index 0 unused).  */
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp && n > 0)   /* safety check */
            exit(EXIT_FAILURE);
        cache = tmp;

        /* Compute all new terms from cache_size+1 up to n.  */
        for (idx_t m = cache_size + 1; m <= n; ++m)
        {
            if (m == 1)         /* first term is given */
            {
                cache[1] = (seq_t)1;
                continue;
            }

            /* n in the recurrence is m-1.  */
            idx_t nn = m - 1;

            /* Start with 4 * a(m-1).  */
            seq_t value = (seq_t)4 * cache[m - 1];

            /* Convolution part: sum_{k=1}^{nn-1} C(nn, k) * a(k) * a(nn-k).  */
            seq_t binom = (seq_t)1;   /* binom(nn, 0) */
            for (idx_t k = 1; k <= nn - 1; ++k)
            {
                /* Update binom to C(nn, k) using integer arithmetic.  */
                binom = binom * (seq_t)(nn - k + 1) / (seq_t)k;

                /* term = binom * a(k) * a(nn-k) */
                seq_t term = binom;
                term *= cache[k];
                term *= cache[nn - k];

                value += term;
            }

            cache[m] = value;
        }

        cache_size = n;
    }

    return cache[n];
}
