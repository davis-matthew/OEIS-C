/*
 * A035006  Number of possible rook moves on an n X n chessboard.
 * https://oeis.org/A035006
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = 2 * (n - 1) * n^2 (OEIS A035006)
 * Range:     u8 n <= 5, u16 n <= 32, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert index to seq_t */
    seq_t sn = n;          /* n as seq_t */
    seq_t res = sn - 1;    /* (n-1) */
    res *= sn;             /* (n-1)*n */
    res *= sn;             /* (n-1)*n^2 */
    res *= 2;              /* 2*(n-1)*n^2 */
    return res;
}
