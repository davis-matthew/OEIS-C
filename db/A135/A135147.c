/*
 * A135147  A binomial recursion : a(n) = p(n) (see formula).
 * https://oeis.org/A135147
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σₖ₌₁ⁿ⁻¹ (2 + C(n,k)) · a(k), a(1)=1 where C(n,k) is the
 *            binomial coefficient
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute binomial coefficient iteratively: start with C(n,1)=n,
   then C(n,k+1) = C(n,k)·(n-k)/(k+1).  */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for already computed values. */
    static seq_t *cache   = NULL;   /* array a[1..] */
    static size_t  cap    = 0;      /* allocated size */
    static idx_t   max_n  = 0;      /* highest n computed so far */

    /* Handle request for n < 1 (should not occur). */
    if (n < 1)
        return 0;

    /* Initial allocation and seed with a(1)=1. */
    if (cache == NULL) {
        cap = 2;                      /* at least space for indices 0 and 1 */
        cache = (seq_t *)calloc(cap, sizeof(seq_t));
        cache[1] = 1;                  /* a(1) */
        max_n = 1;                     /* last computed index */
    }

    /* Grow the cache if necessary. */
    if ((size_t)n >= cap) {
        size_t old_cap = cap;
        while ((size_t)n >= cap)
            cap = cap * 2 + 1;          /* avoid too small growth */
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        /* Zero-initialize the newly allocated part. */
        if (cap > old_cap)
            memset(cache + old_cap, 0,
                   (cap - old_cap) * sizeof(seq_t));
    }

    /* If already computed, return the value. */
    if (n <= max_n)
        return cache[n];

    /* Compute all values up to n, filling the cache. */
    for (idx_t m = max_n + 1; m <= n; ++m) {
        seq_t sum   = 0;
        seq_t binom = (seq_t)m;        /* C(m,1) */

        for (idx_t k = 1; k <= m - 1; ++k) {
            seq_t term = (2 + binom) * cache[k];
            sum += term;

            /* Update binom to C(m,k+1).  Exact division is guaranteed. */
            binom = (binom * (seq_t)(m - k)) / (seq_t)(k + 1);
        }
        cache[m] = sum;
    }

    max_n = n;
    return cache[n];
}
