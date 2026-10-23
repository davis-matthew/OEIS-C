/*
 * A171475  a(n) = 6*a(n-1) - 8*a(n-2), for n > 2, with a(0) = 1, a(1) = 6,
 *          a(2) = 27.
 * https://oeis.org/A171475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - 8*a(n-2) * Initial values: a(0)=1, a(1)=6,
 *            a(2)=27
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)27;

    /* Use recurrence: for n > 2 */
    seq_t prev2 = (seq_t)6;   /* a(1) */
    seq_t prev1 = (seq_t)27;  /* a(2) */

    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t next = (seq_t)6 * prev1 - (seq_t)8 * prev2;
        prev2 = prev1;
        prev1 = next;
    }
    return prev1;
}
