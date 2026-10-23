/*
 * A035290  Number of ways to place a non-attacking white and black pawn on n
 *          X n chessboard.
 * https://oeis.org/A035290
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n <= 2; * a(3) = 6; * a(n) = n^4 - 4 n^3 + n^2 + 10
 *            n - 6 for n >= 4
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 257, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2)
        return 0;
    if (n == 3)
        return 6;

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;     /* n^2   */
    seq_t n3 = n2 * nn;     /* n^3   */
    seq_t n4 = n3 * nn;     /* n^4   */

    seq_t result = n4 - 4 * n3 + n2 + 10 * nn - 6;
    return result;
}
