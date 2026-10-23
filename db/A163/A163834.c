/*
 * A163834  a(n) = (4^n + 5)/3.
 * https://oeis.org/A163834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4^n + 5)/3 = 5*a(n-1) - 4*a(n-2); a(0) = 2, a(1) = 3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
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

    seq_t prev2 = (seq_t)2;   /* a(0) */
    seq_t prev1 = (seq_t)3;   /* a(1) */
    seq_t curr;

    for (idx_t i = 2; i <= n; ++i)
    {
        curr = (seq_t)5 * prev1 - (seq_t)4 * prev2;
        prev2 = prev1;
        prev1 = curr;
    }

    return prev1;
}
