/*
 * A164660  Numerators of row sums of triangle of rationals A164658/A164659.
 *          Definite integral of Chebyshev polynomials of the first kind:
 *          Integral_{x=0..1} T(n,x).
 * https://oeis.org/A164660
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, frac
 * Formula:   a(n) = 1 if n==0 or n%4==1, otherwise a(n) = -1
 * Range:     i8 n <= 100, i16 n <= 100, i32 n <= 100, i64 n <= 100,
 *            i128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)          return (seq_t)1;
    if ((n % 4) == 1)    return (seq_t)1;
    return (seq_t)-1;
}
