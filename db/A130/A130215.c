/*
 * A130215  Order of modular group of degree 7^(n-1)+1.
 * https://oeis.org/A130215
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^(n-1)*(7^(2n-2)-1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Compute 7^(n-1) */
    seq_t pow_val = seq_pow((seq_t)7, n - 1);

    /* 7^(2n-2) = (7^(n-1))^2 */
    seq_t pow_sq = pow_val * pow_val;

    /* (7^(2n-2) - 1) */
    seq_t delta = pow_sq - (seq_t)1;

    /* 7^(n-1) * (7^(2n-2) - 1) */
    seq_t prod = pow_val * delta;

    /* Divide by 2 */
    return prod / (seq_t)2;
}
