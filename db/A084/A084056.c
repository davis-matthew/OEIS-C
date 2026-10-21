/*
 * A084056  a(n) = -a(n-1) + a(n-2) + a(n-3), with a(0)=0, a(1)=1, a(2)=-3.
 * https://oeis.org/A084056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (1/4) * ((1-6*n) * (-1)^n - 1)
 * Range:     i8 n <= 21, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* (-1)^n: 1 for even n, -1 for odd n */
    seq_t sign = (n & 1) ? (seq_t)(-1) : (seq_t)1;
    seq_t temp = (seq_t)1 - (seq_t)6 * (seq_t)n; /* (1-6*n) */
    seq_t term = (sign * temp - (seq_t)1) / (seq_t)4;
    return term;
}
