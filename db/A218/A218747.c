/*
 * A218747  a(n) = (44^n - 1)/43.
 * https://oeis.org/A218747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (44^n - 1)/43 * Recurrence: a(0)=0, a(1)=1;
 *            a(n)=45*a(n-1) - 44*a(n-2)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms directly. */
    if (!n) return 0;
    if (n == 1) return 1;

    /* Use the recurrence to compute a(n). */
    seq_t prev2 = 0;               /* a(0)     */
    seq_t prev1 = 1;               /* a(1)     */
    seq_t cur   = 0;
    for (idx_t i = 2; i <= n; ++i)
    {
        cur = prev1 * (seq_t)45 - prev2 * (seq_t)44;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
