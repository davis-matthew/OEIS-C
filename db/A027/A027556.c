/*
 * A027556  Unbalanced strings of length n.
 * https://oeis.org/A027556
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 6*a(n-3) + 4*a(n-4) (n >= 4) Initial values:
 *            a(0) = 0, a(1) = 0, a(2) = 0, a(3) = 2
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)         /* a(0), a(1), a(2) */
        return 0;
    if (n == 3)        /* a(3) */
        return 2;

    /* Keep the last four values: a(n-4), a(n-3), a(n-2), a(n-1) */
    seq_t prev0 = 0;   /* a(n-4)   */
    seq_t prev1 = 0;   /* a(n-3)   */
    seq_t prev2 = 0;   /* a(n-2)   */
    seq_t prev3 = 2;   /* a(n-1)   */

    /* Compute values from a(4) up to a(n) */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t cur = (seq_t)3 * prev3
                    - (seq_t)6 * prev1
                    + (seq_t)4 * prev0;

        /* Shift the window for the next iteration */
        prev0 = prev1;
        prev1 = prev2;
        prev2 = prev3;
        prev3 = cur;
    }

    return prev3;   /* a(n) */
}
