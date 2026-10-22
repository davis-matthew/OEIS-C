/*
 * A004058  Expansion of (1-x)/( (1+x)*(1-2*x)*(1-3*x)*(1-4*x)*(1-5*x)).
 * https://oeis.org/A004058
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/180)*(2500*5^n - 3456*4^n + 1215*3^n - 80*2^n +
 *            (-1)^n); computed as the expansion of
 *            (1-x)/((1+x)*(1-2x)*(1-3x)*(1-4x)*(1-5x)): c(n) = [x^n]
 *            1/((1-2x)(1-3x)(1-4x)(1-5x)) via h_k(n) = k*h_k(n-1) +
 *            h_{k-1}(n), d(n) = c(n) - d(n-1), d(-1) = 0, a(n) = d(n) -
 *            d(n-1)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t h2 = 0, h3 = 0, h4 = 0, h5 = 0;
    seq_t dprev = 0, d = 0, res = 0;
    idx_t i;
    for (i = 0; i <= n; i++) {
        h2 = (seq_t)2 * h2;
        if (i == 0) h2 = h2 + 1;
        h3 = (seq_t)3 * h3 + h2;
        h4 = (seq_t)4 * h4 + h3;
        h5 = (seq_t)5 * h5 + h4;
        d = h5 - dprev;
        res = d - dprev;
        dprev = d;
    }
    return res;
}
