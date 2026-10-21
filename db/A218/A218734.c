/*
 * A218734  a(n) = (31^n - 1)/30.
 * https://oeis.org/A218734
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (31^n - 1) / 30 (OEIS A218734) Recurrence: a(0) = 0, a(1)
 *            = 1, a(n) = 32*a(n-1) - 31*a(n-2) for n > 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two base cases */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Iterate using the recurrence */
    seq_t prev2 = (seq_t)0;   /* a(n-2)   */
    seq_t prev1 = (seq_t)1;   /* a(n-1)   */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        /* cur = 32*prev1 - 31*prev2   */
        cur = ((seq_t)32 * prev1) - ((seq_t)31 * prev2);
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;   /* a(n) */
}
