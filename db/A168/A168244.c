/*
 * A168244  a(n) = 1 + 3*n - 2*n^2.
 * https://oeis.org/A168244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 + 3*n - 2*n^2
 * Range:     i8 n <= 8, i16 n <= 128, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    long long sn = (long long)n;
    long long val = 1 + 3 * sn - 2 * sn * sn;
    return (seq_t)val;
}
