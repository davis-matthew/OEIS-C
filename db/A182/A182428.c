/*
 * A182428  a(n) = 2n(19-n).
 * https://oeis.org/A182428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 2*n*(19-n)
 * Range:     i8 n <= 4, i16 n <= 137, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (seq_t)n * ((seq_t)19 - (seq_t)n) * 2;
}
