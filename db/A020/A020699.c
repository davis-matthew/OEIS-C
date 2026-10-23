/*
 * A020699  Expansion of (1-3*x)/(1-5*x).
 * https://oeis.org/A020699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1; for n>0, a(n) = 2*5^(n-1) (equivalently a(1)=2,
 *            a(n)=5*a(n-1) for n>=2)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 */

    seq_t res = (seq_t)2;         /* a(1) = 2 */

    for (idx_t i = 1; i < n; ++i) /* multiply by 5 for each step beyond the first term */
        res *= (seq_t)5;

    return res;
}
