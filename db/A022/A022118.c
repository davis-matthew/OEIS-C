/*
 * A022118  Fibonacci sequence beginning 2, 17.
 * https://oeis.org/A022118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 2, a(1) = 17
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)17;

    seq_t prev2 = (seq_t)2;   /* a(n-2) */
    seq_t prev1 = (seq_t)17;  /* a(n-1) */

    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t cur = prev2 + prev1;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}
