/*
 * A098500  Number of squares on infinite quarter chessboard at <=n knight
 *          moves from the corner.
 * https://oeis.org/A098500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n^2 + n + 2)/2 for n > 3 a(0) = 1 a(1) = 3 a(2) = 12
 *            a(3) = 32
 * Range:     u8 n <= 5, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)12;
    if (n == 3) return (seq_t)32;

    seq_t nn = (seq_t)n;
    /* compute (7*nn*nn + nn + 2) / 2 */
    seq_t term = (seq_t)7 * nn * nn + nn + (seq_t)2;
    return term / (seq_t)2;
}
