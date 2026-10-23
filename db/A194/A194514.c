/*
 * A194514  First coordinate of (3,4)-Lagrange pair for n.
 * https://oeis.org/A194514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 3*n - 4*floor((5*n + 3)/7)
 * Range:     i8 n <= 91, i16 n <= 91, i32 n <= 91, i64 n <= 91, i128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t val = 3 * N - 4 * ((5 * N + 3) / 7);
    return val;
}
