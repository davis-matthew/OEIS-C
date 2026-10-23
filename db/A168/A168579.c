/*
 * A168579  G.f.: 1/(1-x-16*x^2).
 * https://oeis.org/A168579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 1, a(n) = a(n-1) + 16 * a(n-2) for n > 1
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)1;
    seq_t prev2 = (seq_t)1;  /* a(0) */
    seq_t prev1 = (seq_t)1;  /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = prev1 + (seq_t)16 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
