/*
 * A001716  Generalized Stirling numbers, [n+5,5]_4.
 * https://oeis.org/A001716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n+1) = (n+5)*a(n) + 4*g(n), g(n+1) = (n+5)*g(n), a(0) = 1,
 *            g(0) = 1 (g(n) = (n+4)!/24; derived from E.g.f. (1 +
 *            4*log(1/(1-x)))/(1-x)^5, which satisfies (1-x)*F' = 5*F +
 *            4/(1-x)^5)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t v = 1;
    seq_t g = 1;
    idx_t i;
    for (i = 0; i < n; i++) {
        seq_t m = (seq_t)i + 5;
        v = m * v + 4 * g;
        g = g * m;
    }
    return v;
}
