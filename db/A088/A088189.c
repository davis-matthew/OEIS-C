/*
 * A088189  Expansion of e.g.f.: 1/(1-sinh(x)-x-x^2).
 * https://oeis.org/A088189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   A088189: expansion of the exponential generating function
 *            1/(1-sinh(x)-x-x^2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/* helper functions from the utils package */
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* The function a(idx_t n) returns the n-th term of A088189.
 * It uses the recurrence
 *
 *   a(0) = 1
 *   a(n) = Σ_{k=1..n} binom(n, k) * f(k) * a(n-k)   for n ≥ 1
 *
 * where the coefficients f(k) are
 *
 *   f(1) = 2,  f(2) = 2
 *   f(k) = 1 for odd k ≥ 3
 *   f(k) = 0 otherwise
 *
 * The computation is performed over the abstract type seq_t; the same
 * code works for fixed‐size integer types as well as for arbitrary
 * precision integer classes.
 */
seq_t a(idx_t n)
{
    /* allocate space for the sequence 0..n */
    seq_t *seq = seq_alloc_new_array((size_t)(n + 1));
    if (!seq) return 0;                  /* allocation failure */
    seq[0] = 1;                          /* a(0) */

    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum = 0;
        seq_t binom = 1;                 /* binom(m, 0) */

        for (idx_t k = 1; k <= m; ++k) {
            /* update binom(m, k) = binom(m, k-1) * (m-k+1) / k */
            binom = binom * ((seq_t)(m - k + 1)) / k;

            /* determine f(k) */
            seq_t fk = 0;
            if (k == 1 || k == 2)
                fk = 2;
            else if ((k & 1U) && k >= 3)
                fk = 1;
            if (fk == 0) continue;        /* skip zero coefficient */

            /* term = binom * fk * a(m-k) */
            seq_t term = binom * fk;
            term = term * seq[m - k];
            sum += term;
        }
        seq[m] = sum;
    }

    seq_t result = seq[n];
    seq_free_array(seq);
    return result;
}
