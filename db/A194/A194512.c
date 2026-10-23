/*
 * A194512  First coordinate of (2,7)-Lagrange pair for n.
 * https://oeis.org/A194512
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*n - 7*floor((5*n + 2)/9)
 * Range:     i8 n <= 92, i16 n <= 92, i32 n <= 92, i64 n <= 92, i128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t flo = (5 * N + 2) / 9;
    return 4 * N - 7 * flo;
}
