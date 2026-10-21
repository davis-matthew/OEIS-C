/*
 * A056220  a(n) = 2*n^2 - 1.
 * https://oeis.org/A056220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*n^2 - 1
 * Range:     i8 n <= 8, i16 n <= 128, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    return ((seq_t)2 * tn * tn) - ((seq_t)1);
}
