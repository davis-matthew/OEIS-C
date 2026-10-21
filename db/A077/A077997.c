/*
 * A077997  Expansion of (1-x)/(1-2*x-x^2-x^3).
 * https://oeis.org/A077997
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + a(n-3), a(0)=1, a(1)=1, a(2)=3
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)3;

    /* iterate up to n, keeping only the last three terms */
    seq_t prev3 = (seq_t)1;   /* a(n-3) */
    seq_t prev2 = (seq_t)1;   /* a(n-2) */
    seq_t prev1 = (seq_t)3;   /* a(n-1) */
    seq_t cur   = (seq_t)0;   /* a(n) */

    for (idx_t i = 3u; i <= n; ++i) {
        cur = (seq_t)2 * prev1 + prev2 + prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
