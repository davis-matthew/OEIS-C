/*
 * A171237  a(0)=2, a(1)=3, a(n) = 3 + a(n-1) + a(n-2) for n >= 2.
 * https://oeis.org/A171237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 2, a(1) = 3, a(n) = 3 + a(n-1) + a(n-2) for n >= 2
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;
    if (n == 1)
        return (seq_t)3;

    seq_t prev2 = (seq_t)2;
    seq_t prev1 = (seq_t)3;
    seq_t cur = prev1;            /* placeholder; will be overwritten in loop */

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = (seq_t)3 + prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
