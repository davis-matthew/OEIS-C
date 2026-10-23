/*
 * A015209  Sum of Gaussian binomial coefficients for q=18.
 * https://oeis.org/A015209
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2)*((18^(n-1))-1), a(0)=1, a(1)=2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_mul.h"   /* seq_mul(seq_t, seq_t) multiplies two values */

seq_t a(idx_t n)
{
    /* static cache for memoisation: cache[i] holds a(i) for i < cache_sz */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 0;
    static idx_t  cache_sz  = 0;               /* next free index (highest+1) */
    static seq_t  pow18 = 0;                    /* 18^(i-1) for the next term */

    /* initialise on first call */
    if (cache == NULL) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;            /* allocation failure */

        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)2;   /* a(1) */
        cache_sz = 2;          /* next free index */
        pow18   = (seq_t)1;    /* 18^(1-1) = 1 */
    }

    /* already computed? */
    if (n < cache_sz)
        return cache[n];

    /* grow cache if necessary */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 2;
        if (new_cap < (size_t)(n + 1))
            new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;              /* allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute all missing terms up to n */
    for (idx_t i = cache_sz; i <= n; ++i) {
        /* pow18 now becomes 18^(i-1) */
        pow18 = seq_mul(pow18, (seq_t)18);

        seq_t term_sub = pow18 - (seq_t)1;          /* (18^(i-1)) - 1 */

        seq_t term = seq_mul(cache[i-1], (seq_t)2)   /* 2*a(i-1) */
                     + seq_mul(cache[i-2], term_sub); /* + a(i-2)*(18^(i-1)-1) */

        cache[i] = term;
    }
    cache_sz = n + 1;
    return cache[n];
}
