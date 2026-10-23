/*
 * A153257  a(n) = n^3 - (n+1)^2.
 * https://oeis.org/A153257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^3 - (n+1)^2
 * Range:     i8 n <= 5, i16 n <= 32, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;
    return (tn * tn * tn) - ((tn + (seq_t)1) * (tn + (seq_t)1));
}
