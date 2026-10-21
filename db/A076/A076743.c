/*
 * A076743  Nonzero coefficients of the polynomials in the numerator of
 *          1/(1+x^2) and its successive derivatives, starting with the
 *          highest power of x.
 * https://oeis.org/A076743
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabf, easy
 * Formula:   a(n) = the n-th term of the concatenated list of non‑zero
 *            coefficients of the numerators of successive derivatives of
 *            1/(1+x^2). The value at position n corresponds to the k-th
 *            coefficient of the d‑th derivative where d is chosen so that
 *            C(d−1) ≤ n < C(d), and k = d−2·(n−C(d−1)). Reference: OEIS...
 * Range:     i8 n <= 6, i16 n <= 15, i32 n <= 32, i64 n <= 32, i128 n <= 32,
 *            big any n
 * Helpers:   binom, mul_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/mul_seq_t.h"

/* Return the n‑th term of the sequence */
seq_t a(idx_t n)
{
    /* Find derivative order d and position r within that derivative */
    idx_t i   = n;          /* zero‑based index in concatenated list */
    idx_t d   = 0;          /* current derivative index */
    idx_t cum = 0;          /* cumulative terms up to derivative d‑1 */

    while (cum + ((d/2)+1) <= i)
    {
        cum += (d/2)+1;
        d++;
    }
    /* r = offset inside derivative d (0 = highest power) */
    idx_t r = i - cum;
    idx_t k = d - 2*r;              /* power of x in this coefficient */

    /* factorial(d) */
    seq_t fact = 1;
    for (idx_t t = 2; t <= d; ++t)
        fact = mul_seq_t(fact, (seq_t)t);

    /* binomial(d+1, k) */
    seq_t bin = binom(d+1, k);

    /* coefficient before sign */
    seq_t coeff = mul_seq_t(fact, bin);

    /* sign = (−1)^(d−r)  (because (d+k)/2 = d−r) */
    if ((d - r) & 1)
        coeff = -coeff;

    return coeff;
}
