/*
 * A035008  Total number of possible knight moves on an (n+2) X (n+2)
 *          chessboard, if the knight is placed anywhere.
 * https://oeis.org/A035008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = 8*n*(n+1)
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = nn * (nn + 1);
    res *= 8;
    return res;
}
