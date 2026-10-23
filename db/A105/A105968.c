/*
 * A105968  a(n) = 4*a(n-1) - a(n-2) - 2*(-1)^n, a(0) = 1, a(1) = 4.
 * https://oeis.org/A105968
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - a(n-2) - 2*(-1)^n, a(0) = 1, a(1) = 4
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the first two terms explicitly */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)4;

    /* Iterative recurrence using O(n) time.
       prev2 = a(n-2), prev1 = a(n-1) */
    seq_t prev2 = (seq_t)1;      /* a(0) */
    seq_t prev1 = (seq_t)4;      /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        /* Compute 4*prev1 - prev2 first */
        cur = (seq_t)(4) * prev1 - prev2;

        /* Adjust with the alternating term +2 or -2 */
        if (i & 1)          /* n is odd => subtract -2 => add 2 */
            cur += 2;
        else
            cur -= 2;        /* n even => subtract 2 */

        /* Shift the window */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
