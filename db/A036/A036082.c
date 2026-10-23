/*
 * A036082  E.g.f.: exp((exp(p*x)-p-1)/p+exp(x)) for p=12.
 * https://oeis.org/A036082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the exponential generating function
 *            exp((exp(12*x)-13)/12 + exp(x)); Recurrence derived from the
 *            EGF: a(0) = 1, a(n) = Σ_{k=1}^{n} (12^{k-1}+1) * C(n-1,k-1) *
 *            a(n-k) (n ≥ 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 8, u64 n <= 14, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* number of allocated slots            */
    static size_t len   = 0;   /* highest index with a value + 1       */

    /* fast path: value already computed */
    if (n < len) {
        return cache[n];
    }

    /* grow cache if necessary */
    if (n + 1 > cap) {
        size_t newcap = ((n + 1) * 2);
        /* realloc with explicit cast so it works in C++ */
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap   = newcap;
    }

    /* initialise a(0) if not yet done */
    if (len == 0) {
        cache[0] = (seq_t)1;
        len = 1;
    }

    /* compute values from len up to n */
    for (size_t i = len; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        seq_t pow12 = (seq_t)1;     /* 12^(k-1) , start with k=1   */
        seq_t binomC = (seq_t)1;    /* C(i-1, k-1) , start with k=1 */

        for (size_t k = 1; k <= i; ++k) {
            /* term = (12^{k-1}+1) * C(i-1,k-1) * a(i-k) */
            seq_t term = cache[i - k];          /* a(i-k)           */
            term *= binomC;                     /* * C(i-1,k-1)     */
            term *= (pow12 + (seq_t)1);         /* * (12^{k-1}+1)   */
            sum += term;

            /* prepare for next k, if any */
            if (k < i) {
                pow12 *= (seq_t)12;                              /* 12^k          */
                /* binomC = binom(i-1, k) = binom(i-1, k-1) * (i-k) / k */
                binomC = binomC * (seq_t)(i - k) / (seq_t)k;
            }
        }
        cache[i] = sum;
    }
    len = n + 1;
    return cache[n];
}
