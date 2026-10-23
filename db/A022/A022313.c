/*
 * A022313  a(n) = a(n-1) + a(n-2) + 1, with a(0) = 0 and a(1) = 8.
 * https://oeis.org/A022313
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + 1, a(0) = 0, a(1) = 8
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 43, u64 n <= 89, u128 n <= 181,
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
        return (seq_t)8;

    /* Use the recurrence iteratively; no memoization needed. */
    seq_t prev2 = (seq_t)0;   /* a(0) */
    seq_t prev1 = (seq_t)8;   /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur   = prev1 + prev2 + (seq_t)1;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
