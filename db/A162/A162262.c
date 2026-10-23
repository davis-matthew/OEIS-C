/*
 * A162262  a(n) = (2*n^3 + 5*n^2 - 13*n)/2.
 * https://oeis.org/A162262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (2*n^3 + 5*n^2 - 13*n)/2
 * Range:     i8 n <= 4, i16 n <= 31, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term = (((seq_t)2 * nn * nn * nn) + ((seq_t)5 * nn * nn) - ((seq_t)13 * nn)) / 2;
    return term;
}
