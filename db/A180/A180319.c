/*
 * A180319  Total number of possible standard knight moves on an n X 2n
 *          chessboard, if the knight is placed anywhere.
 * https://oeis.org/A180319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 for n = 1, otherwise a(n) = 4 * (4 - 9*n + 4*n^2)
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)0;

    seq_t nn   = (seq_t)n;
    seq_t nn2  = nn * nn;                      /* n^2, fits in seq_t for n <= 10^4  */
    seq_t val  = (seq_t)4 - (seq_t)9 * nn + (seq_t)4 * nn2;
    return (seq_t)4 * val;
}
