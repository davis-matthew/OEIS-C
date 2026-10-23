/*
 * A028875  a(n) = n^2 - 5.
 * https://oeis.org/A028875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^2 - 5
 * Range:     i8 n <= 11, i16 n <= 181, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n*n - 5 in the proper integer type seq_t */
    seq_t nn = (seq_t) n;
    return nn * nn - 5;
}
