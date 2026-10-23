/*
 * A119522  Determinant of n X n matrix of first n^2 nonzero terms of
 *          triangular numbers.
 * https://oeis.org/A119522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 1 for n=1; -8 for n=2; -27 for n=3; 0 for n≥4
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)-8;
    if (n == 3) return (seq_t)-27;
    return (seq_t)0;
}
