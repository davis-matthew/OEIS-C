/*
 * A191341  a(n) = 4^n - 2*2^n + 3.
 * https://oeis.org/A191341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - 2^(n+1) + 3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n = 0 an arbitrary
       value is returned; the caller should not request a(0). */
    if (n == 0)
        return (seq_t)0;

    seq_t pow4 = pow_uint(4, n);        /* 4^n  */
    seq_t pow2 = pow_uint(2, n + 1);    /* 2^(n+1) */
    return pow4 - pow2 + (seq_t)3;
}
