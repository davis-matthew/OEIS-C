/*
 * A089148  Expansion of e.g.f.: 1/(exp(x) - x).
 * https://oeis.org/A089148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = -\sum_{k=2}^{n}\binom{n}{k}\,a(n-k), a(0)=1, a(1)=0
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 15, i64 n <= 22, i128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  upto  = 0;

    /* Initialize on first call */
    if (upto == 0) {
        size_t init = 2;                      /* capacity for indices 0 and 1 */
        cache = (seq_t *)realloc(NULL, init * sizeof(seq_t));
        cap   = init;
        cache[0] = 1;
        cache[1] = 0;
        upto    = 2;
    }

    /* Grow cache if necessary */
    if (n >= (idx_t)cap) {
        size_t newcap = cap;
        while ((idx_t)newcap <= n) newcap *= 2;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap   = newcap;
    }

    /* Compute missing terms */
    for (idx_t i = upto; i <= n; ++i) {
        seq_t sum = 0;
        seq_t C   = 1;   /* C(i,0) */
        for (idx_t k = 1; k <= i; ++k) {
            C = C * (seq_t)(i - k + 1) / (seq_t)k;   /* C(i,k) */
            if (k >= 2) {
                sum += C * cache[i - k];
            }
        }
        cache[i] = -sum;
    }
    upto = n + 1;

    return cache[n];
}
