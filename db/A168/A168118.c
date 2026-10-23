/*
 * A168118  a(n) = n*(n^9+1)/2.
 * https://oeis.org/A168118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^9+1)/2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 90, u128 n <= 7643,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;                /* convert index to seq_t   */
    seq_t n9  = pow_uint((uint64_t)n, 9);/* n^9                       */
    seq_t tmp = n9 + (seq_t)1;           /* n^9 + 1                   */

    /* one of nn or tmp is even; divide the even factor by 2 first to avoid overflow */
    if ((tmp & (seq_t)1) == 0)            /* tmp is even              */
        return nn * (tmp >> 1);
    else                                  /* nn is even                */
        return (nn >> 1) * tmp;
}
