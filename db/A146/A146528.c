/*
 * A146528  a(0) = 4; for n >= 1, a(n) = 2^n + 4.
 * https://oeis.org/A146528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(0)=4; for n>=1, a(n)=2^n+4
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)4;
    seq_t val = pow_int((seq_t)2, n);  /* compute 2^n */
    return val + (seq_t)4;
}
