/*
 * A108306  Expansion of (3*x+1)/(1-3*x-3*x^2).
 * https://oeis.org/A108306
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1; a(1)=6; a(n)=3*a(n-1)+3*a(n-2) */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)6;
    seq_t prev2 = (seq_t)1;      /* a(0) */
    seq_t prev1 = (seq_t)6;      /* a(1) */
    seq_t cur = 0;
    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 * (seq_t)3 + prev2 * (seq_t)3;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
