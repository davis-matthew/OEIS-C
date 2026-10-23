/*
 * A163148  a(n) = 16*a(n-1) - 59*a(n-2) for n > 1; a(0) = 1, a(1) = 13.
 * https://oeis.org/A163148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-1) - 59*a(n-2) (a(0)=1, a(1)=13)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 ) return (seq_t)1;
    if (n == 1 ) return (seq_t)13;

    seq_t prev2 = (seq_t)1; /* a(n-2)  */
    seq_t prev1 = (seq_t)13;/* a(n-1)  */
    seq_t cur   = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = 16 * prev1 - 59 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
