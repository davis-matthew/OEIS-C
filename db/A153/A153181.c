/*
 * A153181  Number of overpartitions of n with even rank minus the number of
 *          overpartitions of n with odd rank.
 * https://oeis.org/A153181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 + 2 * Σ_{m≥1} q^{m(m+1)/2} / (
 *            (1+q)(1+q^2)…(1+q^{m-1})(1+q^m)^2 ) (GF in OEIS)
 * Range:     i8 n <= 50, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Validated: partial, pass, 51 terms through n = 50 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation computes the series coefficients up to n by
   progressively building the partial reciprocal polynomials.  It is
   correct for any seq_t (signed integer or arbitrary‑precision). */
#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    size_t N = (size_t)n;
    if (N == 0) return 1;                       /* a(0) */

    /* storage for the answer and the partial reciprocal polynomials */
    seq_t *ans = mem_new(seq_t, N + 1);
    seq_t *p   = mem_new(seq_t, N + 1);
    seq_t *tmp = mem_new(seq_t, N + 1);
    if (!ans || !p || !tmp) {                 /* unlikely */
        mem_free(ans); mem_free(p); mem_free(tmp);
        return 0;
    }

    ans[0] = 1;                                 /* offset term */
    p[0]   = 1;                                 /* start with 1 */

    /* -------- m = 1: reciprocal of (1+q)^2 -------- */
    {
        size_t max_t = N / 1;
        /* clear tmp */
        for (size_t i = 0; i <= N; ++i) tmp[i] = 0;

        for (size_t t = 0; t <= max_t; ++t) {
            seq_t coeff = (t & 1) ? -(seq_t)(t + 1) : (seq_t)(t + 1);
            size_t shift = t;
            size_t limit = N - shift;
            for (size_t i = 0; i <= limit; ++i)
                tmp[i + shift] += p[i] * coeff;
        }
        /* p now holds 1/(1+q)^2 */
        for (size_t i = 0; i <= N; ++i) p[i] = tmp[i];

        /* add contribution for m = 1, T1 = 1 */
        size_t T = 1;
        size_t limit = N - T;
        for (size_t d = 0; d <= limit; ++d)
            ans[T + d] += (seq_t)2 * p[d];
    }

    /* -------- subsequent m (>= 2) -------- */
    for (size_t m = 2; ; ++m) {
        size_t T = m * (m + 1) / 2;
        if (T > N) break;          /* further terms do not affect a(0..N) */

        /* multiply current p by (1 + q^{m-1})  */
        for (size_t i = N; i >= (m - 1); --i)
            p[i] += p[i - (m - 1)];

        /* convolve with (1 + q^m)^{-2} */
        for (size_t i = 0; i <= N; ++i) tmp[i] = 0;

        size_t max_t = N / m;
        for (size_t t = 0; t <= max_t; ++t) {
            seq_t coeff = (t & 1) ? -(seq_t)(t + 1) : (seq_t)(t + 1);
            size_t shift = t * m;
            size_t limit = N - shift;
            for (size_t i = 0; i <= limit; ++i)
                tmp[i + shift] += p[i] * coeff;
        }

        for (size_t i = 0; i <= N; ++i) p[i] = tmp[i];

        /* add contribution for this m */
        size_t limit = N - T;
        for (size_t d = 0; d <= limit; ++d)
            ans[T + d] += (seq_t)2 * p[d];
    }

    seq_t result = ans[N];
    mem_free(ans); mem_free(p); mem_free(tmp);
    return result;
}
