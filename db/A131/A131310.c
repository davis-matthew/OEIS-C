/*
 * A131310  O.g.f. A(x) satisfies: [x^n] exp(x*A(x)) = [x^n] A(x) / n!.
 * https://oeis.org/A131310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(n+1)=n!*sum_{k=0..n} (k+1)/(n-k)!*a(k)*a(n-k)   */
#include <stdlib.h>

/* Static cache for computed a(n) values.  */
static seq_t *a_cache     = NULL;
static idx_t a_cache_len  = 0;   /* number of entries currently stored */
static idx_t a_cache_cap  = 0;   /* allocated capacity of a_cache */

/* Extend the cache so that it contains a(0)...a(n).  */
static void ensure_a_cache(idx_t n)
{
    if (a_cache_len > n) return;   /* nothing to do */

    /* grow the allocated array if necessary */
    if (a_cache_cap == 0) {
        a_cache_cap = 1;
        a_cache = (seq_t*)malloc(sizeof(seq_t));
    }
    while (a_cache_cap <= n) a_cache_cap <<= 1;
    if (a_cache_cap > a_cache_len) {
        seq_t *tmp = (seq_t*)realloc(a_cache, a_cache_cap * sizeof(seq_t));
        a_cache = tmp;
    }

    /* compute all missing a-values */
    for (idx_t m = a_cache_len; m <= n; ++m) {
        if (m == 0) {                 /* a(0)=1 */
            a_cache[0] = (seq_t)1;
            continue;
        }
        idx_t nn = m - 1;              /* n in the recurrence */
        seq_t sum = 0;
        seq_t prod = 1;                /* prod = n!/(n-k)! for current k */

        for (idx_t k = 0; k <= nn; ++k) {
            seq_t term = a_cache[k];
            term *= a_cache[nn - k];
            term *= (seq_t)(k + 1);
            term *= prod;
            sum += term;
            if (k < nn) prod *= (seq_t)(nn - k);   /* prepare prod for next k */
        }
        a_cache[m] = sum;
    }
    a_cache_len = n + 1;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (a_cache_len == 0) {                 /* initialise cache with a(0)=1 */
        a_cache_cap = 1;
        a_cache = (seq_t*)malloc(sizeof(seq_t));
        a_cache[0] = (seq_t)1;
        a_cache_len = 1;
    }

    if (a_cache_len <= n) ensure_a_cache(n);
    return a_cache[n];
}
