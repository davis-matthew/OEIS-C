/*
 * A005253  Number of binary words of length n in which the ones occur only in
 *          blocks of length at least 4.
 * https://oeis.org/A005253
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-5), a(0) = a(1) = a(2) = a(3) =
 *            1, a(4) = 2 (from G.f. (1-x+x^4)/(1-2*x+x^2-x^5))
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 53, u64 n <= 106,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t w0, w1, w2, w3, w4, next;
    idx_t i;
    if (n < 4) return (seq_t)1;
    if (n == 4) return (seq_t)2;
    /* w0 = a(i-5), w1 = a(i-4), w2 = a(i-3), w3 = a(i-2), w4 = a(i-1) */
    w0 = (seq_t)1;
    w1 = (seq_t)1;
    w2 = (seq_t)1;
    w3 = (seq_t)1;
    w4 = (seq_t)2;
    for (i = 5; i <= n; i++) {
        next = w4 + w4;
        next += w0;
        next -= w3;
        w0 = w1;
        w1 = w2;
        w2 = w3;
        w3 = w4;
        w4 = next;
    }
    return w4;
}
