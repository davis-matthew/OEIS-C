/*
 * A214731  a(n) = n^3 - 2*n^2 - 1.
 * https://oeis.org/A214731
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^3 - 2*n^2 - 1
 * Range:     i8 n <= 5, i16 n <= 32, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast the index to the sequence type before arithmetic to avoid
       accidental promotion to a smaller type. */
    seq_t nn = (seq_t)n;
    return nn * nn * nn - 2 * nn * nn - (seq_t)1;
}
