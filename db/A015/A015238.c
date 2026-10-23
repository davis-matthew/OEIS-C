/*
 * A015238  a(n) = (2*n - 3)n^2.
 * https://oeis.org/A015238
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (2*n-3)*n^2
 * Range:     i8 n <= 4, i16 n <= 25, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t sq = sn * sn;           /* n^2 */
    seq_t coef = ( (seq_t)2 * sn ) - (seq_t)3;  /* 2*n - 3 */
    return coef * sq;            /* (2*n-3)*n^2 */
}
