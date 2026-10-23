/*
 * A194518  First coordinate of (3,7)-Lagrange pair for n.
 * https://oeis.org/A194518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 5*n - 7*floor((7*n+3)/10)
 * Range:     i8 n <= 93, i16 n <= 93, i32 n <= 93, i64 n <= 93, i128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t t = 5 * N - 7 * ((7 * N + 3) / 10);
    return t;
}
