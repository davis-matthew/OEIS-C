/*
 * A217924  a(n) = n! * [x^n] exp(2*exp(x) - x - 2). Row sums of triangle
 *          A217537.
 * https://oeis.org/A217924
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n ≥ 1, a(n) = 2*∑_{k=0}^{n-1} binom(n-1,k) * a(k)
 *            – a(n-1)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 23, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from a(n+1)=2∑_{k=0}^{n} C(n,k)a(k)-a(n) */

#include <stdlib.h>
#include <stdint.h>

static seq_t *a_cache = NULL;   /* cache of computed a(n) */
static size_t  a_cap   = 0;     /* allocated capacity of a_cache */
static size_t  a_sz    = 0;     /* number of entries currently stored */

/* make sure a_cache has space for at least `needed` elements */
static void ensure_capacity(size_t needed, size_t *cap, seq_t **ptr)
{
    if (*cap < needed) {
        size_t newcap = needed * 2 + 16;
#if defined(__cplusplus)
        *ptr = reinterpret_cast<seq_t*>(realloc(*ptr, newcap * sizeof(seq_t)));
#else
        *ptr = realloc(*ptr, newcap * sizeof(seq_t));
#endif
        *cap = newcap;
    }
}

seq_t a(idx_t n)
{
    /* grow cache if necessary */
    if ((idx_t)a_sz <= n) {
        ensure_capacity((size_t)n + 1, &a_cap, &a_cache);

        /* initialise first element if the cache was empty */
        if (a_sz == 0) {
            a_cache[0] = (seq_t)1;
            a_sz = 1;
        }

        /* compute missing values up to n */
        for (idx_t i = (idx_t)a_sz; i <= n; ++i) {
            /* sum_{k=0}^{i-1} binom(i-1,k) * a(k) */
            seq_t sum   = (seq_t)0;
            seq_t binom = (seq_t)1;               /* binom(i-1,0) */
            idx_t m = i - 1;                      /* current row of binomial */
            for (idx_t k = 0; k <= m; ++k) {
                sum += binom * a_cache[k];
                if (k < m) {
                    /* binom(i-1,k+1) = binom * (m-k)/(k+1) */
                    binom = binom * (seq_t)(m - k) / (idx_t)(k + 1);
                }
            }
            /* recurrence: a(i) = 2*sum - a(i-1) */
            seq_t val = (seq_t)2 * sum - a_cache[i - 1];
            a_cache[i] = val;
        }
        a_sz = n + 1;
    }
    return a_cache[n];
}
