/*
 * A126958  Define an array by d(m, 0) = 1, d(m, 1) = m; d(m, k) = (m - k + 1)
 *          d(m+1, k-1) - (k-1) (m+1) d(m+2, k-2). Sequence gives d(n,4).
 * https://oeis.org/A126958
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (n+3)*(n+1)*(n^2 -10*n +12)
 * Range:     i8 n <= 2, i16 n <= 15, i32 n <= 216, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t1 = nn + 3;
    seq_t t2 = nn + 1;
    seq_t t3 = nn * nn - 10 * nn + 12;
    return t1 * t2 * t3;
}
