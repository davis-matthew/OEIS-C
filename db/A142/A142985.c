/*
 * A142985  a(1) = 1, a(2) = 6, a(n+2) = 6*a(n+1) + (n + 1)*(n + 2)*a(n).
 * https://oeis.org/A142985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 6, a(n+2) = 6*a(n+1) + (n + 1)*(n + 2)*a(n)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle small indices directly */
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)6;

    /* iterate using the recurrence to avoid large recursion depth */
    seq_t prev2 = (seq_t)1;   /* a(1) */
    seq_t prev1 = (seq_t)6;   /* a(2) */
    seq_t cur = 0;

    for (idx_t k = 3; k <= n; ++k) {
        /* a(k) = 6*a(k-1) + (k-1)*k*a(k-2) */
        cur = (seq_t)6 * prev1 + (seq_t)(k - 1) * (seq_t)k * prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
