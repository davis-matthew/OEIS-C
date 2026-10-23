/*
 * A030434  Values of Newton-Gregory forward interpolating polynomial
 *          (1/3)*(2*n-7)*(2*n^2-11*n+18).
 * https://oeis.org/A030434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1/3)*(2*n-7)*(2*n^2-11*n+18)
 * Range:     i8 n <= 6, i16 n <= 23, i32 n <= 35, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t1 = 2 * nn - 7;                        /* 2n-7  */
    seq_t t2 = 2 * nn * nn - 11 * nn + 18;       /* 2n^2-11n+18 */
    seq_t prod = t1 * t2;                        /* product   */
    return prod / 3;                             /* exact division */
}
