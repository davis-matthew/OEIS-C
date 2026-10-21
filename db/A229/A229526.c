/*
 * A229526  The c coefficients of the transform a*x^2 + (4*a/k - b)*x +
 *          4*a/k^2 + 2*b/k + c = 0 for a,b,c = 1,-1,-1, k = 1,2,3...
 * https://oeis.org/A229526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (-5+3*(-1)^n)*(-4-2*n+n^2)/8
 * Range:     i8 n <= 6, i16 n <= 108, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* (-1)^n : 1 if n is even, -1 if n is odd */
    seq_t parity = ( (n & 1) ? (seq_t)(-1) : (seq_t)1 );

    /* term1 = (-5 + 3*(-1)^n) */
    seq_t term1 = (seq_t)-5 + (seq_t)3 * parity;

    /* n as seq_t */
    seq_t nt = (seq_t)n;

    /* term2 = n^2 - 2*n - 4 */
    seq_t term2 = nt * nt - (seq_t)2 * nt - (seq_t)4;

    /* product divisible by 8 */
    seq_t prod = term1 * term2;

    return prod / (seq_t)8;
}
