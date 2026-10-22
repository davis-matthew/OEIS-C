/*
 * A001242  Differences of reciprocals of unity.
 * https://oeis.org/A001242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = h_{n-1}(24,30,40,60,120), complete homogeneous symmetric
 *            polynomial, from G.f.: x /
 *            ((1-24*x)*(1-30*x)*(1-40*x)*(1-60*x)*(1-120*x)). Computed via
 *            s_j(k) = s_{j-1}(k) + r_j * s_j(k-1), s_j(0) = 1, a(n) =
 *            s_5(n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t s1 = 1, s2 = 1, s3 = 1, s4 = 1, s5 = 1;
    idx_t k;
    for (k = 1; k < n; k++) {
        s1 = s1 * 24;
        s2 = s1 + s2 * 30;
        s3 = s2 + s3 * 40;
        s4 = s3 + s4 * 60;
        s5 = s4 + s5 * 120;
    }
    return s5;
}
