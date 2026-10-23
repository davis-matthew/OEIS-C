/*
 * A103115  a(n) = 6*n*(n-1) - 1.
 * https://oeis.org/A103115
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 6*n*(n-1) - 1
 * Range:     i8 n <= 5, i16 n <= 74, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t un = (seq_t)n;
    return un * (un - 1) * 6 - 1;
}
