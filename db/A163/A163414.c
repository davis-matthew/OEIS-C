/*
 * A163414  a(n) = 16*a(n-1) - 62*a(n-2) for n>1, a(0)=1, a(1)=12.
 * https://oeis.org/A163414
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*a(n-1) - 62*a(n-2), a(0)=1, a(1)=12
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
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
        return (seq_t)12;
    seq_t prev2 = (seq_t)1;   /* a(0) */
    seq_t prev1 = (seq_t)12;  /* a(1) */
    seq_t cur;
    for (idx_t i = 2; i <= n; ++i)
    {
        cur = (seq_t)16 * prev1 - (seq_t)62 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
