/*
 * A099525  Expansion of 1/(1-2x-3x^3).
 * https://oeis.org/A099525
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1)+3*a(n-3); a(0)=1, a(1)=2, a(2)=4
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 2;
    if (n == 2) return 4;
    seq_t prev3 = 1;   /* a(0)   */
    seq_t prev2 = 2;   /* a(1)   */
    seq_t prev1 = 4;   /* a(2)   */
    seq_t cur;
    for (idx_t i = 3; i <= n; ++i) {
        cur = 2 * prev1 + 3 * prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
