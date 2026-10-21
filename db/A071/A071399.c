/*
 * A071399  Rounded volume of a regular tetrahedron with edge length n.
 * https://oeis.org/A071399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = round(n^3 * sqrt(2)/12)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 28, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;          /* cast index to seq_t */
    seq_t n3 = sn * sn;           /* n^2   */
    n3 *= sn;                     /* n^3   */
    seq_t n6 = n3 * n3;           /* (n^3)^2 = n^6 */
    seq_t twon6 = n6 + n6;        /* 2 * n^6 */
    seq_t sq = floor_sqrt(twon6); /* floor(sqrt(2 * n^6)) */
    seq_t res = (sq + (seq_t)6) / (seq_t)12;
    return res;
}
