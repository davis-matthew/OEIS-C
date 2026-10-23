/*
 * A164021  a(n) = 12*a(n-1) - 34*a(n-2) for n > 1; a(0) = 3, a(1) = 22.
 * https://oeis.org/A164021
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 12*a(n-1) - 34*a(n-2), a(0)=3, a(1)=22
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle base cases */
    if (n == 0) return 3;
    if (n == 1) return 22;

    /* Use two previous terms for recurrence */
    seq_t prev2 = 3;   /* a(n-2) */
    seq_t prev1 = 22;  /* a(n-1) */
    seq_t cur;

    /* Iteratively compute up to n */
    for (idx_t i = 2; i <= n; ++i)
    {
        cur = (seq_t)12 * prev1 - (seq_t)34 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
