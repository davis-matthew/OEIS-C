/*
 * A126719  a(n) = -n^2 + 9*n + 23.
 * https://oeis.org/A126719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -n^2 + 9*n + 23
 * Range:     i8 n <= 17, i16 n <= 185, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return -sn * sn + (seq_t)9 * sn + (seq_t)23;
}
