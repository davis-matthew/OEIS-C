/*
 * A005403  Number of protruded partitions of n with largest part at most 2.
 * https://oeis.org/A005403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. (1-x)^2/((1-2x+x^3)(1-x-x^2+x^5)) - 1 =
 *            (1-x)/((1-x-x^2)(1-x-x^2+x^5)) - 1, giving a(n) = 2a(n-1) +
 *            a(n-2) - 2a(n-3) - a(n-4) - a(n-5) + a(n-6) + a(n-7), with c(0)
 *            = 1 (the constant term) and c(k) = 0 for k < 0, a(1) = 1, a(2) =
 *            3, a(3) = 5
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* w[k] holds c(i-1-k) for k = 0..6 */
    seq_t w0 = 1, w1 = 0, w2 = 0, w3 = 0, w4 = 0, w5 = 0, w6 = 0;
    seq_t cur;
    idx_t i;
    if (n <= 1) return (seq_t)1;
    /* c(1) = 2*c(0) - 1 = 1 */
    cur = (seq_t)1;
    w6 = w5; w5 = w4; w4 = w3; w3 = w2; w2 = w1; w1 = w0; w0 = cur;
    for (i = 2; i <= n; i++) {
        seq_t pos = w0 + w0 + w1 + w5 + w6;
        seq_t neg = w2 + w2 + w3 + w4;
        cur = pos - neg;
        w6 = w5; w5 = w4; w4 = w3; w3 = w2; w2 = w1; w1 = w0; w0 = cur;
    }
    return cur;
}
