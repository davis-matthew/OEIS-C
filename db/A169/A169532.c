/*
 * A169532  Number of reduced words of length n in Coxeter group on 39
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^34 = I.
 * https://oeis.org/A169532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the generating function:
 *   G(t) = (t^34 + 2*t^33 + 2*t^32 + … + 2*t + 1) /
 *          (703*t^34 - 37*t^33 - 37*t^32 - … - 37*t + 1)
 *
 * Let a(0)=1. Then for n>0
 *   a[n] = 37*a[n-1] + 37*a[n-2] + … + 37*a[n-33]
 *          - 703*a[n-34] + p[n] ,
 * where p[n] is the coefficient of t^n in the numerator
 * (p[0]=1, p[1..33]=2, p[34]=1, and p[n]=0 for n>34).
 *
 * This implementation caches previously computed terms so that each
 * new term is computed in O(1) time (overall O(n) for the first n terms)
 * and requires only a small amount of memory.
 */

#include <stdlib.h>

/* static cache for previously computed terms */
static seq_t *cache     = NULL;   /* array of terms */
static size_t cache_size = 0;     /* number of terms stored so far */
static size_t cache_cap  = 0;     /* allocated capacity of 'cache' */

/* Ensure that 'cache' has capacity at least 'new_cap' */
static void ensure_cache_cap(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;                               /* already large enough */

    /* allocate new array */
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        return;                               /* allocation failed – caller will handle it */

    /* copy old terms */
    for (size_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];

    /* free old array */
    free(cache);

    cache      = new_cache;
    cache_cap  = new_cap;
}

/* Sequence function */
seq_t a(idx_t n)
{
    size_t target = (size_t)n;          /* target index (n) */

    /* make sure we have capacity for index 'n' */
    if (target >= cache_cap)
        ensure_cache_cap(target + 1);

    /* initialize a[0] if not yet done */
    if (cache_size == 0)
    {
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* compute missing terms up to 'n' */
    for (size_t k = cache_size; k <= target; ++k)
    {
        seq_t sum = (seq_t)0;
        size_t max_i = (k < 34) ? k : 34;          /* i = 1 … max_i */

        /* 37 * a[k-1] + 37 * a[k-2] + … + 37 * a[k-33] */
        for (size_t i = 1; i <= max_i; ++i)
        {
            if (i == 34)                          /* subtract 703 * a[k-34] */
            {
                sum -= (seq_t)703 * cache[k - 34];
            }
            else                                  /* add 37 * a[k-i] */
            {
                sum += (seq_t)37 * cache[k - i];
            }
        }

        /* add numerator coefficient p[k] (only for k ≤ 34) */
        if (k <= 34)
        {
            if (k == 0 || k == 34)
                sum += (seq_t)1;
            else
                sum += (seq_t)2;
        }

        cache[k] = sum;
    }

    cache_size = target + 1;
    return cache[target];
}
