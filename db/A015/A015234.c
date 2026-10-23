/*
 * A015234  a(n) = (17 - 2*n)*n^2.
 * https://oeis.org/A015234
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (17 - 2*n)*n^2
 * Range:     i8 n <= 3, i16 n <= 28, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t val = (seq_t)17 - (seq_t)2 * sn;
    val = val * sn * sn;
    return val;
}
