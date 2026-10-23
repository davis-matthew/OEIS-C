/*
 * A194524  First coordinate of (4,7)-Lagrange pair for n.
 * https://oeis.org/A194524
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*n - 7*floor((3*n + 4)/11)
 * Range:     i8 n <= 95, i16 n <= 95, i32 n <= 95, i64 n <= 95, i128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    __int128 N = n;
    __int128 q = (3 * N + 4) / 11;   /* floor division for positive values */
    __int128 res = 2 * N - 7 * q;
    return (seq_t)res;
}
