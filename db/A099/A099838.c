/*
 * A099838  Expansion of (1-x)^2*(1+x)/(1+x+x^2).
 * https://oeis.org/A099838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0)=1, a(1)=-2; for n>=2 a(n) follows a 3‑cycle: a(3k)=3,
 *            a(3k+1)=-3, a(3k+2)=0 (k>=1)
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)-2;
    if ((n % 3) == 0) return (seq_t)3;
    if ((n % 3) == 1) return (seq_t)-3;
    return (seq_t)0;          /* (n % 3) == 2 */
}
