/*
 * A209440  G.f.: 1 = Sum_{n>=0} a(n)*x^n * (1-x)^((n+1)^2).
 * https://oeis.org/A209440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(n)=Sum_{k=0..n-1}
 *            (-1)^(n+1-k)*a(k)*binomial((k+1)^2,n-k) for n>=1. Uses
 *            memoization; relies on binomial(idx_t,idx_t) from utils
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* static cache for sequence values */
    static seq_t *cache = NULL;  /* array of computed a(k) */
    static size_t cache_sz = 0;  /* allocated size of the array */
    static idx_t computed = 0;   /* highest index that has been computed */

    /* enlarge cache if it is not large enough */
    if ((size_t)(n + 1) > cache_sz) {
        size_t new_sz = (size_t)(n + 1);
        seq_t *new_mem = (seq_t *)malloc(new_sz * sizeof(seq_t));
        if (new_mem == NULL)
            return 0;   /* allocation failure */
        /* copy existing values */
        for (size_t i = 0; i < cache_sz; ++i)
            new_mem[i] = cache[i];
        /* initialize new slots to zero (will be overwritten) */
        for (size_t i = cache_sz; i < new_sz; ++i)
            new_mem[i] = 0;
        free(cache);
        cache = new_mem;
        cache_sz = new_sz;
    }

    /* initialize a(0) once */
    if (computed == 0) {
        cache[0] = 1;
        computed = 0;
    }

    if (n == 0)
        return cache[0];

    /* compute any missing values up to n */
    for (idx_t i = computed + 1; i <= n; ++i) {
        seq_t pos = 0;
        seq_t neg = 0;

        for (idx_t k = 0; k < i; ++k) {
            idx_t r = i - k;
            idx_t m = (k + 1) * (k + 1);   /* (k+1)^2 */
            if (r > m)
                continue;          /* binomial would be zero */

            seq_t bin = binomial(m, r);
            seq_t prod = cache[k] * bin;

            /* sign = (-1)^(i+1-k) */
            if (((i + 1 - k) & 1) == 0)     /* even ⇒ positive sign */
                pos += prod;
            else
                neg += prod;
        }
        cache[i] = pos - neg;
        computed = i;
    }

    return cache[n];
}
