/*
 * A127978  a(n) = ((15*n + 34)/54)*2^(n-1) - (-1)^(n-1)*(6*n + 5)/27.
 * https://oeis.org/A127978
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((15*n + 34)/54)*2^(n-1) - (-1)^(n-1)*(6*n + 5)/27
 *            Equivalent linear recurrence of order 4: a(n) = 2*a(n-1) +
 *            3*a(n-2) - 4*a(n-3) - 4*a(n-4) with initial terms a(2) = 3, a(3)
 *            = 5, a(4) = 15, a(5) = 31
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 60, u128 n <= 123,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2) return 0;           /* not defined for n < 2 (offset = 2) */

    /* return the documented initial values directly */
    if (n == 2) return (seq_t)3;
    if (n == 3) return (seq_t)5;
    if (n == 4) return (seq_t)15;
    if (n == 5) return (seq_t)31;

    /* iterate the recurrence from n = 6 up to the requested n */
    seq_t prev4 = (seq_t)3,   /* a(n-4) */
           prev3 = (seq_t)5,   /* a(n-3) */
           prev2 = (seq_t)15,  /* a(n-2) */
           prev1 = (seq_t)31;  /* a(n-1) */
    seq_t cur;

    for (idx_t i = 6; i <= n; ++i) {
        /* a(i) = 2*a(i-1) + 3*a(i-2) - 4*a(i-3) - 4*a(i-4)   */
        cur = 2 * prev1 + 3 * prev2 - 4 * prev3 - 4 * prev4;

        /* shift the window of last four values */
        prev4 = prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;  /* at loop exit prev1 holds a(n) */
}
