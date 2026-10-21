/*
 * A062158  a(n) = n^3 - n^2 + n - 1 = (n-1) * (n^2 + 1).
 * https://oeis.org/A062158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^3 - n^2 + n - 1
 * Range:     i8 n <= 5, i16 n <= 32, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    long long sn = (long long)n;
    long long v  = sn * sn * sn - sn * sn + sn - 1;
    return (seq_t)v;
}
