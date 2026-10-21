/*
 * A214965  Least m > 0 such that for every r and s in the set S = {{h*e} : h
 *          = 1,..,n} of fractional parts, if r < s, then r < k/m < s for some
 *          integer k; m is the least separator of S as defined at A001000.
 * https://oeis.org/A214965
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least m>0 such that for all consecutive r<s in the sorted
 *            fractional parts of {h*e : h=1..n} one has ∃ integer k with r <
 *            k/m < s. (OEIS A214965)
 * Range:     u8 n <= 142, u16 n <= 300, u32 n <= 300, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   cmp_long_double
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>          /* for long double operations */
#include "../utils/cmp_long_double.h"

/* Compute the nth term of the sequence.  The function is valid for
   all n >= 2; for n < 2 it returns (seq_t)1 as a placeholder. */
seq_t a(idx_t n) {
    if (n < 2) return (seq_t)1;

    /* Compute fractional parts of h*e for h = 1..n. */
    size_t N = (size_t)n;            /* safe: n fits in size_t */
    long double *frac = (long double *)malloc(N * sizeof(long double));
    if (!frac) return (seq_t)0;      /* allocation failure */

    const long double e = expl(1.0L); /* e with long double precision */
    for (size_t i = 0; i < N; ++i) {
        long double v = (i + 1) * e;
        v -= floorl(v);              /* fractional part only */
        frac[i] = v;
    }

    /* Sort the fractional parts. */
    qsort(frac, N, sizeof(long double), cmp_long_double);

    /* Incrementally test integer denominators m. */
    for (idx_t m = 1; ; ++m) {
        bool ok = true;
        for (size_t i = 0; i + 1 < N; ++i) {
            long double r = frac[i];
            long double s = frac[i + 1];

            /* Evaluate r*m and s*m with long double precision. */
            long double rm = r * (long double)m;
            long double sm = s * (long double)m;

            /* The smallest integer strictly larger than r*m is floor(rm)+1. */
            idx_t k = (idx_t)floorl(rm) + 1;

            /* Check if this integer lies strictly below s*m. */
            if ((long double)k >= sm) {
                ok = false;
                break;
            }
        }
        if (ok) { /* found the least separator */
            free(frac);
            return (seq_t)m;
        }
    }
}
