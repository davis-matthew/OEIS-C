/*
 * A055588  a(n) = 3*a(n-1) - a(n-2) - 1 with a(0) = 1 and a(1) = 2.
 * https://oeis.org/A055588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-2) - 1, with a(0)=1, a(1)=2
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 46, u128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == (idx_t)0) {
        return (seq_t)1;
    }
    if (n == (idx_t)1) {
        return (seq_t)2;
    }
    seq_t prev0 = (seq_t)1;  /* a(0) */
    seq_t prev1 = (seq_t)2;  /* a(1) */
    seq_t cur;
    for (idx_t i = (idx_t)2; i <= n; ++i) {
        cur = (seq_t)3 * prev1 - prev0 - (seq_t)1;
        prev0 = prev1;
        prev1 = cur;
    }
    return prev1;
}
