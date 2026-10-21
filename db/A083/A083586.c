/*
 * A083586  Binomial transform of A083580.
 * https://oeis.org/A083586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0, a(1) = 1, a(2) = 5, a(3) = 22 a(n) = 11*a(n-1) -
 *            43*a(n-2) + 69*a(n-3) - 36*a(n-4) for n ≥ 4 (derived from the
 *            generating function of A083586)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Small n – return the initial terms directly */
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)5;
    if (n == 3)
        return (seq_t)22;

    /* Variables hold the four most recent values:
       prev4 = a(n-4), prev3 = a(n-3), prev2 = a(n-2), prev1 = a(n-1)   */
    seq_t prev4 = (seq_t)0;      /* a(0)   */
    seq_t prev3 = (seq_t)1;      /* a(1)   */
    seq_t prev2 = (seq_t)5;      /* a(2)   */
    seq_t prev1 = (seq_t)22;     /* a(3)   */
    seq_t cur;

    for (idx_t i = 4; i <= n; ++i)
    {
        /* Compute the next term using only addition and subtraction,
           which is safe because
           (11*a(n-1) + 69*a(n-3)) >= (43*a(n-2) + 36*a(n-4)) for all n ≥ 4. */
        seq_t pos = (seq_t)11 * prev1 + (seq_t)69 * prev3;
        seq_t neg = (seq_t)43 * prev2 + (seq_t)36 * prev4;
        cur = pos - neg;

        /* Shift the sliding window */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
