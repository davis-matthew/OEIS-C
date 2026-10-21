/*
 * A083327  a(n) = (5^n - 4^n + 3^n - 2^n)/2.
 * https://oeis.org/A083327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (5^n - 4^n + 3^n - 2^n)/2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Initial terms a(0)=0, a(1)=1, a(2)=7, a(3)=40 */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)7;
    if (n == 3) return (seq_t)40;

    /* Recurrence: a(n)=14*a(n-1)-71*a(n-2)+154*a(n-3)-120*a(n-4)   */
    seq_t prev4 = (seq_t)0;   /* a(n-4)   */
    seq_t prev3 = (seq_t)1;   /* a(n-3)   */
    seq_t prev2 = (seq_t)7;   /* a(n-2)   */
    seq_t prev1 = (seq_t)40;  /* a(n-1)   */
    seq_t cur   = (seq_t)0;

    for (idx_t i = 4; i <= n; ++i)
    {
        /* Compute positive and negative parts separately to avoid
           underflow on unsigned type. */
        seq_t pos = prev1 * (seq_t)14 + prev3 * (seq_t)154;
        seq_t neg = prev2 * (seq_t)71 + prev4 * (seq_t)120;
        cur = pos - neg;

        /* Shift the window for next iteration */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
