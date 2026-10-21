/*
 * A053464  a(n) = n*5^(n-1).
 * https://oeis.org/A053464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*5^(n-1) (with a(0) = 0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   pow5_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow5_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    seq_t pow5 = pow5_uint64(n-1);   /* 5^(n-1) */
    return pow5 * (seq_t)n;          /* n * 5^(n-1) */
}
