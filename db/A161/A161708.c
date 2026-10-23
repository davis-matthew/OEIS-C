/*
 * A161708  a(n) = -n^3 + 7*n^2 - 5*n + 1.
 * https://oeis.org/A161708
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -n^3 + 7*n^2 - 5*n + 1
 * Range:     i8 n <= 8, i16 n <= 34, i32 n <= 1292, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the unsigned index to the sequence type first */
    seq_t x = (seq_t)n;
    /* Compute the polynomial in a safe, type-correct way */
    return -x * x * x + 7 * x * x - 5 * x + 1;
}
