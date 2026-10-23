/*
 * A022119  Fibonacci sequence beginning 2, 19.
 * https://oeis.org/A022119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 2, a(1) = 19, a(n) = a(n-1) + a(n-2) for n ≥ 2 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)19;
    seq_t prev2 = (seq_t)2; /* a(n-2) */
    seq_t prev1 = (seq_t)19;/* a(n-1) */
    seq_t cur = 0;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
