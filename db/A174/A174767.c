/*
 * A174767  y-values in the solution to x^2 - 23*y^2 = 1.
 * https://oeis.org/A174767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 48*a(n-1) - a(n-2), a(1) = 0, a(2) = 5
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)5;

    /* iterate once, keeping only the last two values */
    seq_t prev2 = (seq_t)0;  /* a(1) */
    seq_t prev1 = (seq_t)5;  /* a(2) */
    seq_t cur;

    for (idx_t i = 3; i <= n; ++i) {
        cur = (seq_t)48 * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;   /* a(n) */
}
