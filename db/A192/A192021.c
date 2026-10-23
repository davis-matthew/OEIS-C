/*
 * A192021  The Wiener index of the binomial tree of order n.
 * https://oeis.org/A192021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(n-1)*(1+2^n*(n-1))
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t pow_n_minus1 = pow2(n - 1);
    seq_t pow_n        = pow2(n);
    seq_t term = pow_n * ((seq_t)(n - 1)) + (seq_t)1;
    return pow_n_minus1 * term;
}
