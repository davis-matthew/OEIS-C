/*
 * A022309  a(n) = a(n-1) + a(n-2) + 1 for n>1, a(0)=0, a(1)=4.
 * https://oeis.org/A022309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + 1, a(0)=0, a(1)=4
 * Range:     u8 n <= 9, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 182,
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
        return (seq_t)4;
    seq_t prev2 = (seq_t)0;    /* a(n-2) */
    seq_t prev1 = (seq_t)4;    /* a(n-1) */
    seq_t curr;
    for (idx_t i = 2; i <= n; ++i)
    {
        curr = prev1 + prev2 + (seq_t)1;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
