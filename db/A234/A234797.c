/*
 * A234797  E.g.f. satisfies: A'(x) = 1 + A(x) + 2*A(x)^2, where A(0)=0.
 * https://oeis.org/A234797
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies the differential equation A'(x)=1+A(x)+2*A(x)^2
 *            with A(0)=0. Recurrence (for m≥2): a(1) = 1 a(m) = a(m‑1) + 2 *
 *            Σ_{k=1}^{m‑2} binom(m‑1,k) * a(k) * a(m‑1‑k)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, free */

seq_t a(idx_t n)
{
    /* Return 0 for non‑positive arguments – the sequence is defined for n≥1. */
    if (n < 1)
        return (seq_t)0;

    /* Static cache for previously computed terms. */
    static seq_t *cache = NULL;      /* array of a[0]..a[max] (a[0] unused)  */
    static size_t cache_cap = 0;     /* allocated capacity in number of elements */
    static size_t cache_len = 0;     /* highest index that has been computed  */

    /* Initialise cache on the first call. */
    if (!cache) {
        cache_cap = 16;                      /* start with a small capacity  */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();                 /* out of memory */
        cache[0] = (seq_t)0;                 /* unused, but convenient to keep */
        cache[1] = (seq_t)1;                 /* a(1) = 1 */
        cache_len = 1;
    }

    /* Grow the cache if necessary. */
    if ((size_t)n > cache_cap - 1) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;           /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute all terms up to n that are not yet cached. */
    for (idx_t m = (idx_t)(cache_len + 1); m <= n; ++m) {
        /* Compute the inner summation. */
        seq_t sum = (seq_t)0;
        seq_t C   = (seq_t)1;                     /* starting binomial (m-1 choose 0) =1 */
        /* We need k from 1 to m-2; terms for k=0 or k=m-1 are zero because a[0]=0. */
        for (idx_t k = 1; k <= m - 2; ++k) {
            /* Update binomial coefficient: C = C * (m-1-(k-1)) / k  */
            C = (C * (seq_t)(m - 1 - (k - 1))) / (seq_t)k;

            seq_t term = cache[k] * cache[m - 1 - k];
            term = term * C;
            sum += term;
        }

        cache[m] = cache[m - 1] + (seq_t)2 * sum;
    }

    cache_len = (size_t)n;
    return cache[(size_t)n];
}
