/*
 * A123968  a(n) = n^2 - 3, starting at n=1.
 * https://oeis.org/A123968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n^2 - 3
 * Range:     i8 n <= 11, i16 n <= 50, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n must be >= 1 for the defined part of the sequence.  For n==0
       we return 0 as a harmless placeholder. */
    if (n == 0) return (seq_t)0;
    seq_t nn = (seq_t)n;
    return nn * nn - (seq_t)3;
}
