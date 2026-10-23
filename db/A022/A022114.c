/*
 * A022114  Fibonacci sequence beginning 2 9.
 * https://oeis.org/A022114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 2, a(1) = 9, a(n) = a(n‑1) + a(n‑2) for n >= 2
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)9;

    seq_t prev2 = (seq_t)2;
    seq_t prev1 = (seq_t)9;
    seq_t cur = 0;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev2 + prev1;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
