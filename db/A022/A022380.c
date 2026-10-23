/*
 * A022380  Fibonacci sequence beginning 3, 12.
 * https://oeis.org/A022380
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 3, a(1) = 12
 * Range:     u8 n <= 7, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)12;

    seq_t prev2 = (seq_t)3;   /* a(0) */
    seq_t prev1 = (seq_t)12;  /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur   = prev1 + prev2;  /* a(i) = a(i-1) + a(i-2) */
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
