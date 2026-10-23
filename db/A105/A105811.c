/*
 * A105811  Expansion of g.f. (1+x-x^2)/(1+x)^2.
 * https://oeis.org/A105811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = (-1)^(n+1)*(n-2) - 0^n
 * Range:     i8 n <= 130, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t sign = (n & 1) ? (seq_t)1 : (seq_t)-1;
    seq_t ans = sign * (sn - 2);
    if (n == 0)
        ans -= (seq_t)1;          /* subtract 0^0 = 1 only for n = 0 */
    return ans;
}
