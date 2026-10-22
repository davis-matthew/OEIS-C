/*
 * A004799  Self-convolution of Lucas numbers.
 * https://oeis.org/A004799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3) - a(n-4), a(0) = 0, a(1) =
 *            1, a(2) = 6, a(3) = 17 (from G.f. x*((1+2*x)/(1-x-x^2))^2)
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 37, u64 n <= 82, u128 n <= 172,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t w0 = 0, w1 = 1, w2 = 6, w3 = 17;
    if (n == 0) return w0;
    if (n == 1) return w1;
    if (n == 2) return w2;
    if (n == 3) return w3;
    for (idx_t i = 4; i <= n; i++) {
        seq_t t = w3 + w3 + w2;
        t -= w1 + w1;
        t -= w0;
        w0 = w1;
        w1 = w2;
        w2 = w3;
        w3 = t;
    }
    return w3;
}
