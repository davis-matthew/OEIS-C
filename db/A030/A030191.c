/*
 * A030191  Scaled Chebyshev U-polynomial evaluated at sqrt(5)/2.
 * https://oeis.org/A030191
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-1) - 5*a(n-2) Initial values: a(0) = 1, a(1) = 5
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    /* Working values for the recurrence */
    seq_t prev2 = (seq_t)1;   /* a(n-2), starting with a(0) */
    seq_t prev1 = (seq_t)5;   /* a(n-1), starting with a(1) */
    seq_t cur;
    const seq_t FIVE = (seq_t)5;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = (prev1 * FIVE) - (prev2 * FIVE);
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
