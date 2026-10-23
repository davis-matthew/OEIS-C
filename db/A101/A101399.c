/*
 * A101399  a(0) = 1, a(1) = 2, a(2) = 5; for n >= 3, a(n) = a(n-1) + 2*a(n-2)
 *          + a(n-3).
 * https://oeis.org/A101399
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + a(n-3); a(0)=1, a(1)=2, a(2)=5
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 28, u64 n <= 57, u128 n <= 116,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)5;
    seq_t prev3 = (seq_t)1; /* a(0) */
    seq_t prev2 = (seq_t)2; /* a(1) */
    seq_t prev1 = (seq_t)5; /* a(2) */
    for (idx_t i = 3; i <= n; ++i) {
        seq_t curr = prev1 + (seq_t)2 * prev2 + prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
