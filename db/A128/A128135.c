/*
 * A128135  Row sums of A128134.
 * https://oeis.org/A128135
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1 a(n) = 2^(n-2) * (2*n - 1) for n > 1 (Equivalently:
 *            a(n) = 2*a(n-1) + 2^(n-1) for n >= 3)
 * Range:     u8 n <= 6, u16 n <= 13, u32 n <= 28, u64 n <= 59, u128 n <= 122,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;
    seq_t pow = pow2(n - 2);
    seq_t two_n_minus1 = ((seq_t)n << 1) - 1;  // (2*n - 1)
    return pow * two_n_minus1;
}
