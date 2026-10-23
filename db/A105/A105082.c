/*
 * A105082  Expansion of (5+4x)/(1-2x-x^2).
 * https://oeis.org/A105082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2), a(0)=5, a(1)=14
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 48, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)5;
    if (n == 1) return (seq_t)14;
    seq_t prev2 = (seq_t)5;
    seq_t prev1 = (seq_t)14;
    seq_t curr;
    for (idx_t i = 2; i <= n; ++i) {
        curr = (prev1 + prev1) + prev2;   /* 2*prev1 + prev2 */
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
