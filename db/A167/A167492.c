/*
 * A167492  Number of reduced words of length n in Coxeter group on 38
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^14 = I.
 * https://oeis.org/A167492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* calloc, realloc */

/* 
   OEIS A167492: Number of reduced words of length n in a Coxeter
   group on 38 generators with (S_i)^2 = (S_i S_j)^14 = I.
   
   The generating function is 
       (t^14 + 2t^13 + … + 2t + 1) / (666t^14 - 36t^13 - … - 36t + 1) .
   Hence a linear recurrence of order 14 holds:
       a(n) = 36·Σ_{k=1}^{13} a(n-k) – 666·a(n-14)   for n ≥ 15.
   The first 15 terms (n = 0 … 14) are obtained from the initial
   equations of the generating function and are:
   a(0)=1, a(1)=38, a(2)=1406, a(3)=52022, a(4)=1924814,
   a(5)=71218118, a(6)=2635070366, a(7)=97497603542,
   a(8)=3607411331054, a(9)=133474219248998,
   a(10)=4938546112212926, a(11)=182726206151878262,
   a(12)=6760869627619495694, a(13)=250152176221921340678,
   a(14)=9255630520211089604383.
*/

static seq_t *cache = NULL;      /* array holding computed a(n)   */
static size_t cache_cap = 0;     /* allocated capacity              */
static size_t cache_len = 0;     /* number of initialized entries   */

/* Ensure the cache has room for indices up to n (inclusive). */
static void
ensure_cache_cap(idx_t n)
{
    if ((size_t)n + 1 <= cache_cap)
        return;
    size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
    while (new_cap <= n)
        new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* Recurrence implementation. */
seq_t
a(idx_t n)
{
    /* Grow the cache if necessary. */
    ensure_cache_cap(n);

    /* Nothing to compute yet. */
    if (cache_len == 0) {
        /* Compute the first 15 terms using the equations derived
           from the generating function (for 0 ≤ i ≤ 14). */
        static const unsigned char P[15] =
        {1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1};
        for (idx_t i = 0; i <= 14; ++i) {
            seq_t sum = 0;
            if (i < 14) {
                /* a(i) = P[i] + 36*Σ_{k=1}^{i} a(i-k) */
                for (idx_t k = 1; k <= i; ++k)
                    sum += cache[i - k];
                cache[i] = (seq_t)P[i] + (seq_t)36 * sum;
            } else { /* i == 14 */
                /* a(14) = 1 + 36*Σ_{k=1}^{13} a(14-k) – 666*a(0) */
                for (idx_t k = 1; k <= 13; ++k)
                    sum += cache[14 - k];
                cache[i] = (seq_t)1 + (seq_t)36 * sum - (seq_t)666 * cache[0];
            }
        }
        cache_len = 15; /* we have a(0)…a(14) stored */
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;
        if (i >= 15) {
            /* a(i) = 36*Σ_{k=1}^{13} a(i-k) – 666*a(i-14) */
            for (idx_t k = 1; k <= 13; ++k)
                sum += cache[i - k];
            cache[i] = (seq_t)36 * sum - (seq_t)666 * cache[i - 14];
        } else {
            /* The initial segment was already handled above. */
            /* This branch is reached only if n < 15 and
               cache_len < n+1, which cannot happen because
               cache_len is at least 15 after the initial block. */
        }
    }
    cache_len = n + 1;
    return cache[n];
}
