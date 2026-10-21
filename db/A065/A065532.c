/*
 * A065532  a(n) = 48*n^2 - 1.
 * https://oeis.org/A065532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 48*n^2 - 1
 * Range:     i8 n <= 1, i16 n <= 26, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* Convert index to sequence type first */
    return nn * nn * 48 - 1;      /* Compute 48*n^2 - 1 */
}
