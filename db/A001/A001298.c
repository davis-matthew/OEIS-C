/*
 * A001298  Stirling numbers of the second kind S(n+4, n).
 * https://oeis.org/A001298
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n+7,8) + 22*C(n+6,8) + 58*C(n+5,8) + 24*C(n+4,8), from
 *            G.f. x(1 + 22x + 58x^2 + 24x^3)/(1 - x)^9
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 32, u64 n <= 537, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 973 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t a001298_c8(idx_t m) {
    idx_t k, j;
    seq_t r = 1;
    if (m < 8) return 0;
    k = (m - 8 < 8) ? m - 8 : 8;
    for (j = 1; j <= k; j++) {
        r = r * (seq_t)(m - k + j) / (seq_t)j;
    }
    return r;
}

seq_t a(idx_t n) {
    seq_t s = a001298_c8(n + 7);
    s += 22 * a001298_c8(n + 6);
    s += 58 * a001298_c8(n + 5);
    s += 24 * a001298_c8(n + 4);
    return s;
}
