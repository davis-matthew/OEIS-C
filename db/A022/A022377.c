/*
 * A022377  Fibonacci sequence beginning 2, 30.
 * https://oeis.org/A022377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 2, a(1) = 30
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)2;
    if (n == 1u) return (seq_t)30;
    seq_t prev1 = (seq_t)2;
    seq_t prev2 = (seq_t)30;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = prev1 + prev2;
        prev1 = prev2;
        prev2 = cur;
    }
    return prev2;
}
