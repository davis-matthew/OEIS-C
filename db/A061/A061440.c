/*
 * A061440  Denominators of coefficients of odd powers of x of the expansion
 *          of Bessel function J_9(x).
 * https://oeis.org/A061440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2*n+9) * n! * (n+9)!
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 5, u128 n <= 12,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t prod = 1;
    idx_t i;

    /* compute (n+9)! */
    for (i = 1; i <= n + 9; ++i)
        prod *= (seq_t)i;

    /* multiply by n! again to obtain n! * (n+9)! */
    for (i = 1; i <= n; ++i)
        prod *= (seq_t)i;

    /* multiply by 2^(2*n+9) */
    prod *= pow2(2 * n + 9);

    return prod;
}
