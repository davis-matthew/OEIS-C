/*
 * A057156  Number of functions from {0,1}^n to {0,1}^n.
 * https://oeis.org/A057156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = (2^n)^(2^n)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   pow2_idx, pow_seq_large
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2_idx.h"
#include "../utils/pow_seq_large.h"

seq_t a(idx_t n)
{
    /* 2^n ^ 2^n  = 2^(n * 2^n) */
    seq_t power_of_two = pow2_idx(n);          /* 2^n */
    seq_t exponent = (seq_t)n * power_of_two;  /* n * 2^n */
    return pow_seq_large((seq_t)2, exponent);
}
