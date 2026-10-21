/*
 * A085377  a(n) = 15*n^2 + 13*n^3.
 * https://oeis.org/A085377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*n^2 + 13*n^3
 * Range:     u8 n <= 2, u16 n <= 16, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* cast index to sequence type */
    seq_t sn2 = sn * sn;          /* n^2 */
    seq_t tmp = 15 + 13 * sn;     /* 15 + 13*n */
    return sn2 * tmp;             /* n^2 * (15 + 13*n) */
}
