/*
 * A179297  a(n) = n^2 - (n-1)^2 - (n-2)^2 - ... - 1^2.
 * https://oeis.org/A179297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -n*(1-9*n+2*n^2)/6
 * Range:     i8 n <= 6, i16 n <= 26, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn  = (seq_t)n;            /* cast index to seq_t */
    seq_t nn2 = nn * nn;             /* n^2  */
    seq_t numer = -(nn * (1 - 9 * nn + 2 * nn2)); /* (6*a(n)) */
    return numer / 6;                /* a(n) */
}
