/*
 * A022319  a(n) = a(n-1) + a(n-2) + 1 for n > 1, a(0)=1, a(1)=5.
 * https://oeis.org/A022319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 1 for n > 1, with a(0) = 1, a(1) = 5
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)5;

    seq_t prev2 = (seq_t)1;   /* a(n-2) */
    seq_t prev1 = (seq_t)5;   /* a(n-1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = prev1 + prev2 + (seq_t)1;
        prev2 = prev1;
        prev1 = cur;
    }

    return cur;
}
