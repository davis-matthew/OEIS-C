/*
 * A083905  G.f.: 1/(1-x) * sum(k>=0, (-1)^k*x^2^(k+1)/(1+x^2^k)).
 * https://oeis.org/A083905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(1)=0; a(2n) = -a(n)+1; a(2n+1) = -a(n)
 * Range:     i8 n <= 86, i16 n <= 86, i32 n <= 86, i64 n <= 86, i128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if ((n & 1ULL) == 0ULL)          /* n is even: 2n case */
        return -a(n / 2) + (seq_t)1;
    else                             /* n is odd: 2n+1 case */
        return -a(n / 2);
}
