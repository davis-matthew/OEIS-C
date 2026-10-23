/*
 * A042590  Numerators of continued fraction convergents to sqrt(824).
 * https://oeis.org/A042590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 11, u32 n <= 21, u64 n <= 44, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A042590: Numerators of continued fraction convergents to sqrt(824)
   Continued fraction of sqrt(824) is a0 = 28, with periodic part
   [1,2,2,1,1,6,1,1,2,2,1,56].  
   Recurrence: p(-2)=0, p(-1)=1, p(k)=a_k * p(k-1) + p(k-2) for k≥0. */

/* Expected external types:
   - idx_t : unsigned 64‑bit integer */
#include <stdlib.h>

/* Periodic part of the continued fraction (12 terms) */
static const unsigned int period[12] = {
    1, 2, 2, 1, 1, 6, 1, 1, 2, 2, 1, 56
};

/* Cached computed numerators */
static seq_t *cache = NULL;
static idx_t cacheSize = 0;  /* number of elements stored in cache */

seq_t a(idx_t n)
{
    /* If not yet initialized, compute the first term */
    if (cacheSize == 0) {
        cache = (seq_t *)realloc(cache, sizeof(seq_t));
        cache[0] = 28;            /* a(0) */
        cacheSize = 1;
        if (n == 0) return cache[0];
    }

    if (n < cacheSize) return cache[n];   /* value already known */

    /* Grow cache to hold terms 0 … n */
    cache = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));

    /* Initialise the recurrence with the last two already computed values */
    seq_t prev1 = cache[cacheSize - 1];
    seq_t prev2 = (cacheSize >= 2) ? cache[cacheSize - 2] : (seq_t)1;  /* p(-1) = 1 */

    for (idx_t k = cacheSize; k <= n; ++k) {
        unsigned int a_k;
        if (k == 0) {
            a_k = 28; /* a0, already handled, but kept for completeness */
        } else {
            a_k = period[(k - 1) % 12]; /* periodic part */
        }

        /* Compute p(k) = a_k * p(k-1) + p(k-2) */
        seq_t pk = prev1 * (seq_t)a_k + prev2;

        cache[k] = pk;

        /* Shift for next iteration */
        prev2 = prev1;
        prev1 = pk;
    }

    cacheSize = n + 1;
    return cache[n];
}
