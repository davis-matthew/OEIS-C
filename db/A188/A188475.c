/*
 * A188475  a(n) = (2*n^3 + 3*n^2 + n + 3)/3.
 * https://oeis.org/A188475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n^3 + 3*n^2 + n + 3)/3
 * Range:     u8 n <= 4, u16 n <= 31, u32 n <= 1859, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = n;                      /* cast idx_t to seq_t */
    seq_t n2 = nn * nn;                /* n^2 */
    seq_t n3 = n2 * nn;                /* n^3 */
    seq_t num = 2 * n3 + 3 * n2 + nn + (seq_t)3;
    return num / 3;
}
