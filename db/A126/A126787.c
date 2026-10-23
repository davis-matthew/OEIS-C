/*
 * A126787  G.f.: B(x)*B(2!*x^2)*B(3!*x^3)*..., where B(x) is g.f. of A000142.
 * https://oeis.org/A126787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} Σ_{m≥0} (m! * (k!)^m) *
 *            x^{k m}
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A126787 for any n >= 0. */

#include <stdlib.h>
#include <stdint.h>

/* External types supplied by the build.  Do not define them. */
extern seq_t factorial(idx_t n); /* returns n! as seq_t */

/* Implementation of the sequence. */
seq_t a(idx_t n)
{
    /* Handle trivial case n = 0. */
    if (n == 0) {
        seq_t one = 1;
        return one;
    }

    /* Allocate arrays for DP.  Use calloc to zero-initialize. */
    size_t sz = (size_t)n + 1;
    seq_t *g   = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *tmp = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!g || !tmp) return 0;        /* allocation failed, return 0 */

    g[0] = 1;                        /* g[0] = 1, all other entries 0 */

    seq_t fact = 1;                  /* factorial of current k */
    for (idx_t k = 1; k <= n; ++k) {
        fact *= (seq_t)k;            /* fact = k! */

        /* Zero tmp array for new DP values. */
        for (size_t i = 0; i < sz; ++i) tmp[i] = 0;

        /* Iterate over all current sums. */
        for (idx_t s = 0; s <= n; ++s) {
            seq_t cur = g[s];
            if (cur == 0) continue; /* skip zero entries */

            idx_t max_m = (n - s) / k;
            seq_t w = 1;          /* w_0 = 1 */

            /* m = 0 contribution */
            tmp[s] += cur;          /* cur * 1 */

            /* m >= 1 contributions */
            for (idx_t m = 1; m <= max_m; ++m) {
                /* w_m = w_{m-1} * m * k! */
                w = w * (seq_t)m;
                w = w * fact;
                tmp[s + k * m] += cur * w;
            }
        }

        /* Swap g and tmp for next iteration. */
        seq_t *swap = g;
        g = tmp;
        tmp = swap;
    }

    seq_t result = g[n];
    free(g);
    free(tmp);
    return result;
}
