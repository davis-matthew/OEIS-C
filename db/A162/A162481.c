/*
 * A162481  Expansion of (1/(1-x)^3)*c(x/(1-x)^3), c(x) the g.f. of A000108.
 * https://oeis.org/A162481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A162481:  a(n) = binomial(n+2,2) + Σ_{k=0}^{n-1} a(k)*a(n-1-k)  
 * (equivalently: a(0)=1, a(n)=((n+1)*(n+2))/2 + Σ_{k=0}^{n-1}a(k)*a(n-1-k) for n≥1)
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache for memoisation.  It grows only upwards. */
    static seq_t *cache = NULL;   /* array of previously computed a(i) */
    static size_t cap   = 0;      /* number of elements stored in cache */

    /* If the value is already computed, return it. */
    if ((idx_t)cap > n)
        return cache[(size_t)n];

    /* Extend the cache to hold indices up to n. */
    size_t needed = (size_t)n + 1;
    seq_t *p = (seq_t *)realloc(cache, needed * sizeof(seq_t));
    if (!p && needed > 0)            /* allocation error: give 0 */
        return 0;
    cache = p;

    /* Compute fresh entries (from old cap up to n). */
    for (idx_t i = (idx_t)cap; i <= n; ++i)
    {
        if (i == 0)  /* a(0) = 1 */
        {
            cache[0] = 1;
            continue;
        }

        /*  binomial(i+2,2) = (i+1)*(i+2)/2  */
        seq_t binom = ((seq_t)(i + 1)) * ((seq_t)(i + 2)) / 2;
        seq_t sum   = binom;

        /*  convolution part: Σ_{k=0}^{i-1} a(k)*a(i-1-k)  */
        for (idx_t k = 0; k < i; ++k)
            sum += cache[(size_t)k] * cache[(size_t)(i - 1 - k)];

        cache[(size_t)i] = sum;
    }

    cap = needed;
    return cache[(size_t)n];
}
