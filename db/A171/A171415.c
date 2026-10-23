/*
 * A171415  a(n) = 99*a(n-1) - a(n-2); a(0) = 0, a(1) = 1.
 * https://oeis.org/A171415
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 99*a(n-1) - a(n-2); a(0) = 0, a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t prev2 = (seq_t)0;   /* a(0) */
    seq_t prev1 = (seq_t)1;   /* a(1) */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = prev1 * (seq_t)99 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
