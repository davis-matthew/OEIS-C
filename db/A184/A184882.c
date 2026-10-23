/*
 * A184882  a(n) = 1 - 4*n - 4*n^2.
 * https://oeis.org/A184882
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 1 - 4*n - 4*n^2
 * Range:     i8 n <= 5, i16 n <= 90, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t res = (seq_t)1 - (seq_t)4 * sn - (seq_t)4 * sn * sn;
    return res;
}
