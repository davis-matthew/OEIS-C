/*
 * A049071  Expansion of x*(3-2*x)/(1-x^2).
 * https://oeis.org/A049071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = (1-5*(-1)^n)/2 for n>0, a(0)=0
 * Range:     i8 n <= 79, i16 n <= 79, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    seq_t sign = (n & 1) ? -1 : 1;   /* sign = (-1)^n */
    return (1 - 5 * sign) / 2;
}
