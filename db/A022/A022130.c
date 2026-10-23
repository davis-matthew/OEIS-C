/*
 * A022130  Fibonacci sequence beginning 4,9.
 * https://oeis.org/A022130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2), a(0) = 4, a(1) = 9
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)9;

    seq_t prev2 = (seq_t)4;   /* a(0) */
    seq_t prev1 = (seq_t)9;   /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur   = prev2 + prev1;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
