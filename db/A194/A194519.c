/*
 * A194519  Second coordinate of (3,7)-Lagrange pair for n.
 * https://oeis.org/A194519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n - 3*floor((3*n+6)/10)
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use __int128 to avoid overflow for the intermediate product */
    unsigned __int128 tmp = (unsigned __int128)3 * n + (unsigned __int128)6;
    idx_t t = (idx_t)(tmp / (unsigned __int128)10);
    return (seq_t)n - (seq_t)3 * (seq_t)t;
}
