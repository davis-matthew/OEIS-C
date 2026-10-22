/*
 * A001808  Expansion of 1/((1+x)*(1-x)^12).
 * https://oeis.org/A001808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f. 1/((1+x)*(1-x)^12) = 1/(1-x^2) * 1/(1-x)^11, so a(n) is the
 *            11-fold iterated partial sum of c(m) = [m even], c = 1,0,1,0,...
 *            Equivalently a(n) = Sum_{j=0..floor(n/2)} binomial(n-2j+10, 10)
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 32, u64 n <= 288, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t s[12];
    int i;
    idx_t m;
    for (i = 0; i < 12; i++) s[i] = 0;
    for (m = 0; m <= n; m++) {
        s[0] = (m % 2 == 0) ? (seq_t)1 : (seq_t)0;
        for (i = 1; i < 12; i++) s[i] += s[i - 1];
    }
    return s[11];
}
