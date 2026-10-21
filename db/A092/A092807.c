/*
 * A092807  Expansion of (1-6*x+4*x^2)/((1-2*x)*(1-6*x)).
 * https://oeis.org/A092807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 8*a(n-1) - 12*a(n-2); initial values: a(0)=1, a(1)=2,
 *            a(2)=8
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)8;

    seq_t prev2 = (seq_t)2; /* a(1) */
    seq_t prev1 = (seq_t)8; /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i)
    {
        cur = prev1 * (seq_t)8 - prev2 * (seq_t)12;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
