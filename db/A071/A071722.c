/*
 * A071722  Expansion of (1+x^2*C^2)*C^3, where C = (1-(1-4*x)^(1/2))/(2*x) is
 *          g.f. for Catalan numbers, A000108.
 * https://oeis.org/A071722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*binomial(2*n+2,n) + 5*binomial(2*n,n+2)) / (n+3)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* intermediate indices */
    idx_t two_n        = n + n;        /* 2*n   */
    idx_t two_n_plus_2 = two_n + 2;     /* 2*n+2 */

    /* binomial coefficients using the provided utility */
    seq_t c1 = binomial(two_n_plus_2, n);      /* C(2*n+2, n)   */
    seq_t c2 = binomial(two_n, n + 2);        /* C(2*n,   n+2) */

    /* numerator: 3*C(2*n+2,n) + 5*C(2*n,n+2) */
    seq_t num = (seq_t)3 * c1 + (seq_t)5 * c2;

    /* denominator */
    seq_t den = (seq_t)n + 3U;

    /* exact division – the result is known to be integer */
    return num / den;
}
