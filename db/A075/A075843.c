/*
 * A075843  Numbers k such that 99*k^2 + 1 is a square.
 * https://oeis.org/A075843
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 20*a(n-1) - a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Direct iterative recurrence, fits all unbounded builds and is O(n). */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    seq_t prev2 = (seq_t)0;   /* a(n-2) */
    seq_t prev1 = (seq_t)1;   /* a(n-1) */
    seq_t cur;
    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)(20) * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
