/*
 * A190975  a(n) = 8*a(n-1) - 2*a(n-2), with a(0)=0, a(1)=1.
 * https://oeis.org/A190975
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8*a(n-1) - 2*a(n-2), a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;

    seq_t prev2 = (seq_t)0;  /* a(0) */
    seq_t prev1 = (seq_t)1;  /* a(1) */
    seq_t cur = (seq_t)0;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = 8 * prev1 - 2 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;  /* a(n) */
}
