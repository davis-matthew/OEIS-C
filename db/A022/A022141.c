/*
 * A022141  Fibonacci sequence beginning 5, 17.
 * https://oeis.org/A022141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 5, a(1) = 17
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) {
        return (seq_t)5;
    }
    if (n == 1) {
        return (seq_t)17;
    }

    seq_t prev2 = (seq_t)5;   /* a(0) */
    seq_t prev1 = (seq_t)17;  /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur   = prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
