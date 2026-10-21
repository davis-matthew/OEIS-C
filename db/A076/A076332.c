/*
 * A076332  a(n) = rad(n) + n/rad(n), where rad(n) is the squarefree kernel of
 *          n = A007947(n).
 * https://oeis.org/A076332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = rad(n) + n / rad(n) (squarefree kernel + quotient)
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/rad.h"

seq_t a(idx_t n)
{
    /* rad(n) returns the product of distinct prime factors of n */
    seq_t rad_val = rad(n);
    seq_t sn = (seq_t)n;           /* convert index to seq_t for safe arithmetic */
    seq_t div = sn / rad_val;      /* integer division – exactly n/rad(n) */
    return rad_val + div;          /* sum of the two terms */
}
