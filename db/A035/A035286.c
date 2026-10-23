/*
 * A035286  Number of ways to place a non-attacking white and black king on n
 *          X n chessboard.
 * https://oeis.org/A035286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 - 9 n^2 + 12 n - 4
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;          /* n^2 */
    seq_t term = sq * sq;        /* n^4 */
    term -= (seq_t)9 * sq;       /* -9 n^2 */
    term += (seq_t)12 * nn;      /* +12 n */
    term -= (seq_t)4;            /* -4 */
    return term;
}
