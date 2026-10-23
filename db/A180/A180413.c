/*
 * A180413  Total number of possible knight moves on an n X n X n chessboard,
 *          if the knight is placed anywhere.
 * https://oeis.org/A180413
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = 24*n*(n^2-1)
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t nn = (seq_t)n;
    seq_t temp = nn * nn - 1;
    seq_t res  = nn * temp;
    return res * 24;
}
