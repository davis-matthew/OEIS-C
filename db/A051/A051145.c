/*
 * A051145  a(0)=0, a(1)=1, a(n) = smallest number such that sequence b(n) =
 *          a(n) OR a(n+1) is strictly monotonically increasing.
 * https://oeis.org/A051145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 1276, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = ((a(n-1) OR a(n-2)) + 1) AND NOT a(n-1),  a(0)=0, a(1)=1  */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t prev2 = (seq_t)0;   /* a(n-2) */
    seq_t prev1 = (seq_t)1;   /* a(n-1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t cand = (prev1 | prev2) + (seq_t)1;   /* (a(n-1) | a(n-2)) + 1 */
        seq_t mask = ~(prev1);                     /* NOT a(n-1) */
        cur     = cand & mask;                    /* a(n)   */
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
