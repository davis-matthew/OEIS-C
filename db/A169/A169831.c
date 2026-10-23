/*
 * A169831  a(n) = 5*2^(n+1) - 3*(n+3).
 * https://oeis.org/A169831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*2^(n+1) - 3*(n+3)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* 2^(n+1) as a seq_t value */
    seq_t p = pow_uint(2, n + 1);

    /* 5*2^(n+1) */
    seq_t t1 = p * (seq_t)5;

    /* 3*(n+3) converted to seq_t */
    seq_t t2 = (seq_t)((idx_t)3 * (n + 3));

    return t1 - t2;
}
