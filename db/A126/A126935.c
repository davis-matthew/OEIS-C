/*
 * A126935  Define an array by d(m, 0) = 1, d(m, 1) = m; d(m, k) = (m - k + 1)
 *          d(m+1, k-1) - (k-1) (m+1) d(m+2, k-2). Sequence gives d(n,3).
 * https://oeis.org/A126935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n*(n+2)*(n-5)
 * Range:     i8 n <= 7, i16 n <= 33, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return nn * (nn + 2) * (nn - 5);
}
