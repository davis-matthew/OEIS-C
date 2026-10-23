/*
 * A047009  T(2n,n), array T given by A047000.
 * https://oeis.org/A047009
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A006013(n-1) = 2*(3n-2)!/(n!*(2n-1)!)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;   /* a(0) = 1 */

    idx_t three_n_minus_2 = 3*n - 2;
    idx_t two_n_minus_1   = 2*n - 1;

    /* Compute binomial coefficient C(3n-2, n) */
    seq_t val = binomial(three_n_minus_2, n);
    /* Multiply by 2 and divide by (2n-1) to obtain a(n) */
    val = val * 2;
    val = val / two_n_minus_1;
    return val;
}
