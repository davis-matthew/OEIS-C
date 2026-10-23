/*
 * A022406  a(0)=3, a(1)=7; thereafter a(n) = a(n-1) + a(n-2) + 1.
 * https://oeis.org/A022406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 3, a(1) = 7; a(n) = a(n-1) + a(n-2) + 1
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 181,
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
        return (seq_t)7;

    seq_t prev2 = 3;       /* a(0) */
    seq_t prev1 = 7;       /* a(1) */
    seq_t cur;

    /* iterate from 2 up to n */
    for (idx_t i = 2; i <= n; ++i)
    {
        cur = prev1 + prev2 + (seq_t)1;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
