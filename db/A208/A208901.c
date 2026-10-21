/*
 * A208901  Number of bitstrings of length n (with at least two runs) where
 *          the last two runs have different lengths.
 * https://oeis.org/A208901
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - 2^(floor(n/2)+1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 2^n  */
    seq_t pow_n  = pow2(n);

    /* 2^(floor(n/2)+1)  */
    seq_t pow_half = pow2((n >> 1) + 1);

    return pow_n - pow_half;
}
