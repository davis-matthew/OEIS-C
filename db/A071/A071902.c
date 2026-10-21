/*
 * A071902  Sum_{k=0..n^2} (k^2 - n^2)/n.
 * https://oeis.org/A071902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = n*(2*n^2-5)*(n^2+1)/6
 * Range:     i8 n <= 2, i16 n <= 7, i32 n <= 31, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t to avoid implicit integer conversions */
    seq_t N  = (seq_t)n;          /* n */
    seq_t n2 = N * N;             /* n^2 */

    seq_t t1 = n2 * (seq_t)2 - (seq_t)5;  /* 2*n^2 - 5 */
    seq_t t2 = n2 + (seq_t)1;             /* n^2 + 1 */

    seq_t prod = N * t1 * t2;             /* n * (2*n^2-5) * (n^2+1) */
    return prod / (seq_t)6;               /* divide by 6 */
}
