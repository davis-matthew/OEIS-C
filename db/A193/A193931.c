/*
 * A193931  E.g.f. A(x) = exp(x+x^2+x^3+x^4+x^5).
 * https://oeis.org/A193931
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A193931 – a(n) = n!*sum_{k=1..n} sum_{i=0..(n-k)/5}
 *  Simplified recurrence (derived from the exponential generating function):
 *       a(0)=1
 *       for n≥1
 *       a(n) = a(n-1)
 *              + 2*(n-1)*a(n-2)
 *              + 3*(n-1)*(n-2)*a(n-3)
 *              + 4*(n-1)*(n-2)*(n-3)*a(n-4)
 *              + 5*(n-1)*(n-2)*(n-3)*(n-4)*a(n-5)
 *
 *  This iteration uses only integer arithmetic and works for any
 *  seq_t (fixed‑size or arbitrary precision) supported by the
 *  build system.
 */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static idx_t   next = 0;          /* first index not yet computed */
    static size_t  cap   = 0;          /* allocated slots */

    if (n < next)
        return cache[n];

    /* grow cache if necessary */
    if (n + 1 > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure; caller may handle */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute terms from 'next' up to 'n' */
    for (idx_t i = next; i <= n; ++i) {
        if (i == 0) {
            cache[0] = (seq_t)1;
            continue;
        }
        seq_t sum = cache[i - 1];   /* a(n-1) */

        /* product accumulates (i-1)*(i-2)*… for higher k terms */
        seq_t prod = 1;
        for (int k = 2; k <= 5; ++k) {
            if (i < (idx_t)k) break;       /* term would use negative index */
            /* multiply by (i - k + 1) = (i - (k-1)) */
            prod *= (seq_t)(i - k + 1);
            seq_t term = prod * cache[i - k];
            term *= (seq_t)k;               /* multiply by k */
            sum += term;
        }

        cache[i] = sum;
    }

    next = n + 1;
    return cache[n];
}
