/*
 * A176812  Expansion of 3*(1+x)/(1-2*x-5*x^2).
 * https://oeis.org/A176812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(0) = 3, a(1) = 9, a(n) = 2*a(n-1) + 5*a(n-2) for n ≥ 2
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)3;
    if (n == 1)
        return (seq_t)9;

    seq_t prev0 = (seq_t)3;   /* a(0) */
    seq_t prev1 = (seq_t)9;   /* a(1) */
    seq_t curr  = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        curr = prev1 * (seq_t)2 + prev0 * (seq_t)5;
        prev0 = prev1;
        prev1 = curr;
    }
    return prev1;
}
