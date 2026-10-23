/*
 * A185939  a(n) = 9*n^2 - 6*n + 2.
 * https://oeis.org/A185939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n^2 - 6*n + 2
 * Range:     u8 n <= 5, u16 n <= 85, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* convert index to seq_t */
    seq_t sq = nn * nn;           /* n^2  */
    return 9 * sq - 6 * nn + (seq_t)2;
}
