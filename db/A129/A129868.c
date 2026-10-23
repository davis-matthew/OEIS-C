/*
 * A129868  Binary palindromic numbers with only one 0 bit.
 * https://oeis.org/A129868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 2^(2n+1) - 2^n - 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute the exponent 2n+1 safely */
    idx_t exp = n + n + 1;

    /* 2^n and 2^(2n+1) */
    seq_t pow2_n   = pow2(n);
    seq_t pow2_exp = pow2(exp);

    /* a(n) = 2^(2n+1) - 2^n - 1 */
    seq_t result = pow2_exp - pow2_n - (seq_t)1;
    return result;
}
