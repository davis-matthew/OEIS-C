/*
 * A121000  Numerators of partial sums of Catalan numbers scaled by powers of
 *          1/18^2 = 1/324.
 * https://oeis.org/A121000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   * a(n) = numerator of r(n) where * r(n) = Σ_{k=0}^n C(k)/324^k
 *            (C(k) = Catalan number) * The fraction r(n) is reduced to lowest
 *            terms. * This uses the recurrence * N(0) = C(0) * N(n) = C(n) +
 *            324 * N(n-1) (N(n) = numerator with common denom 324^n) * The
 *            reduced numerator is N(n) divided by all 2’...
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 11,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* Cached numerators for all terms up to the largest requested n */
    static seq_t *N  = NULL;
    static size_t  Nlen = 0;

    /* Grow the cache if necessary */
    if ((idx_t)Nlen <= n) {
        size_t newlen = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(N, newlen * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failure – return 0 safely */
        N = tmp;

        /* Compute new entries */
        for (size_t i = Nlen; i < newlen; ++i) {
            if (i == 0) {
                N[0] = catalan(0);          /* C(0) = 1 */
            } else {
                /* N[i] = C(i) + 324 * N[i-1] */
                N[i] = catalan((idx_t)i) + N[i - 1] * 324U;
            }
        }
        Nlen = newlen;
    }

    /* Reduce N[n] by removing all 2's and 3's that are also in 324^n */
    seq_t val = N[n];
    idx_t twos_needed   = 2 * n;   /* exponent of 2 in 324^n */
    idx_t threes_needed = 4 * n;   /* exponent of 3 in 324^n */

    idx_t twos   = 0;
    while (twos < twos_needed && (val % 2) == 0) {
        val /= 2;
        ++twos;
    }

    idx_t threes = 0;
    while (threes < threes_needed && (val % 3) == 0) {
        val /= 3;
        ++threes;
    }

    return val;   /* reduced numerator */
}
