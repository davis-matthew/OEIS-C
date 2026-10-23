/*
 * A036549  a(n) = T(8,n), array T given by A048471.
 * https://oeis.org/A036549
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 128*3^n - 127
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 15, u64 n <= 35, u128 n <= 76,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t p = pow3(n);               /* 3^n */
    return (seq_t)128 * p - (seq_t)127;
}
