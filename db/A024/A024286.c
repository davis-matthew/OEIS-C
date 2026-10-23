/*
 * A024286  Expansion of e.g.f. log(1+sin(x)*x)/2 (even coefficients).
 * https://oeis.org/A024286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (2n)! * [x^(2n)] log(1+sin(x)*x)/2
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 6, i64 n <= 10, i128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Assuming seq_t and idx_t are defined by the build. */
static seq_t *a_cache = NULL;      /* dynamic cache of computed terms */
static size_t a_cap = 0;           /* current size of the cache */

seq_t a(idx_t n) {
    /* Handle the offset and trivial cases immediately. */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Initialise the cache if this is the first call. */
    if (a_cache == NULL) {
        a_cap = 2;                      /* we already know a(0) and a(1) */
        a_cache = (seq_t *)malloc(a_cap * sizeof(seq_t));
        a_cache[0] = (seq_t)0;
        a_cache[1] = (seq_t)1;
    }

    /* Grow the cache to hold at least n+1 elements. */
    if (n >= a_cap) {
        size_t new_cap = (size_t)(n + 1);
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        for (idx_t i = (idx_t)a_cap; i <= n; ++i) {
            if (i == 0) { a_cache[0] = (seq_t)0; continue; }
            if (i == 1) { a_cache[1] = (seq_t)1; continue; }

            seq_t sum = 0;                 /* sum over t = 1 … n-1 */
            idx_t m = i;                   /* current n */

            /* cur = binom(2m, 2)  = (2m)*(2m-1)/2   */
            seq_t cur = (seq_t)(2*m) * (seq_t)(2*m - 1) / (seq_t)2;

            for (idx_t t = 1; t <= m - 1; ++t) {
                /* term = cur * 2*t / m * (m - t) * a[t] * (-1)^t */
                seq_t term = cur;
                term *= (seq_t)2;
                term *= (seq_t)t;
                term /= (seq_t)m;
                term *= (seq_t)(m - t);
                term *= a_cache[t];
                if (t & 1) term = -term;   /* multiply by (-1)^t */

                sum += term;

                /* Update cur to binom(2m, 2(t+1)) if needed */
                if (t < m - 1) {
                    seq_t num = cur;
                    num *= (seq_t)(2*m - 2*t);
                    num *= (seq_t)(2*m - 2*t - 1);
                    seq_t den = (seq_t)(2*t + 1);
                    den *= (seq_t)(2*t + 2);
                    cur = num / den;
                }
            }

            /* a(m) = (-1)^(m-1) * (m - sum) */
            seq_t temp = (seq_t)m - sum;
            seq_t sign = ((m - 1) & 1) ? -1 : 1;
            a_cache[m] = sign * temp;
        }
        a_cap = new_cap;
    }

    return a_cache[n];
}
