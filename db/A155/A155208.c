/*
 * A155208  G.f.: A(x) = exp( Sum_{n>=1} (4^n + 1)^n * x^n/n ), a power series
 *          in x with integer coefficients.
 * https://oeis.org/A155208
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = exp( Σ_{k≥1} ((4^k+1)^k) * x^k / k ) with a(0)=1. The
 *            recurrence for the ordinary coefficients is: a(0)=1, a(n) =
 *            (1/n) * Σ_{k=1}^n ((4^k+1)^k) * a(n-k)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* static cache of a[0..highest] */
    static seq_t *cache   = NULL;
    static size_t  cache_cap = 0;
    static idx_t   highest = 0;

    /* static cache of b[k] = (4^k+1)^k, stored as b[k-1] for k>=1 */
    static seq_t *b      = NULL;
    static size_t  b_cap   = 0;
    static idx_t   b_highest = 0;

    /* initialise cache for a[0] */
    if (cache == NULL) {
        cache = (seq_t *)malloc(sizeof(seq_t) * 1);
        cache[0] = (seq_t)1;      /* a(0) = 1 */
        cache_cap = 1;
        highest = 0;
    }

    /* ensure cache can hold up to n */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(1);   /* abort on allocation failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* ensure b array can hold up to n (only if n>=1) */
    if ((size_t)n >= b_cap) {
        size_t new_cap = (size_t)n;   /* need indices 0..n-1 for b[1..n] */
        seq_t *tmp = (seq_t *)realloc(b, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(1);
        b = tmp;
        b_cap = new_cap;
    }

    /* compute b[k] for k = b_highest+1 .. n */
    for (idx_t k = b_highest + 1; k <= n; ++k) {
        seq_t pow4  = big_pow((seq_t)4, (seq_t)k);          /* 4^k */
        seq_t base  = pow4 + (seq_t)1;                      /* 4^k + 1 */
        seq_t bk    = big_pow(base, (seq_t)k);             /* (4^k+1)^k */
        b[k - 1] = bk;
    }
    b_highest = n;

    /* compute missing a[j] from highest+1 up to n */
    for (idx_t j = highest + 1; j <= n; ++j) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k <= j; ++k) {
            sum += cache[j - k] * b[k - 1];
        }
        /* division by j is exact */
        cache[j] = sum / (seq_t)j;
    }
    highest = n;

    return cache[n];
}
