/*
 * A090316  a(n) = 24*a(n-1) + a(n-2), starting with a(0) = 2 and a(1) = 24.
 * https://oeis.org/A090316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 24*a(n-1) + a(n-2), a(0) = 2, a(1) = 24
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* handle base cases */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)24;

    /* iterative computation */
    seq_t prev0 = (seq_t)2;   /* a(0) */
    seq_t prev1 = (seq_t)24;  /* a(1) */
    seq_t next;

    for (idx_t i = 2; i <= n; ++i) {
        next = prev1 * (seq_t)24 + prev0;
        prev0 = prev1;
        prev1 = next;
    }
    return prev1;
}
