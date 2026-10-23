/*
 * A162259  a(n) = (2*n^3 + 5*n^2 - 17*n)/2.
 * https://oeis.org/A162259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (2*n^3 + 5*n^2 - 17*n)/2
 * Range:     i8 n <= 3, i16 n <= 24, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Ensure n is interpreted in seq_t to avoid overflow in lower-width types. */
    seq_t sn = (seq_t)n;
    /* Compute the numerator: 2*n^3 + 5*n^2 - 17*n. */
    seq_t num = (seq_t)2 * sn * sn * sn
              + (seq_t)5 * sn * sn
              - (seq_t)17 * sn;
    /* Divide by 2.  The expression is always even for integer n ≥ 1. */
    return num / 2;
}
