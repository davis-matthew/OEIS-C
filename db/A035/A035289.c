/*
 * A035289  Number of ways to place a non-attacking white and black knight on
 *          n X n chessboard.
 * https://oeis.org/A035289
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 - 9*n^2 + 24*n - 16
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;          /* cast index to seq_t first */
    seq_t m2 = m * m;            /* n^2 */
    seq_t res = m2 * m2           /* n^4 */
               - (seq_t)9 * m2     /* 9*n^2 */
               + (seq_t)24 * m     /* 24*n */
               - (seq_t)16;        /* 16 */
    return res;
}
