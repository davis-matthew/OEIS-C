/*
 * A015611  a(n) = 12*a(n-1) + 7*a(n-2).
 * https://oeis.org/A015611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1) + 7*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two seed values explicitly */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Iteratively compute a(n) using two previous terms */
    seq_t prev2 = (seq_t)0;     /* a(0) */
    seq_t prev1 = (seq_t)1;     /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = (seq_t)12 * prev1 + (seq_t)7 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
