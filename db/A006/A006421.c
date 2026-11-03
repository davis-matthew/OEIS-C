/*
 * A006421  Number of rooted planar maps with 4 vertices and n faces and no
 *          isthmuses.
 * https://oeis.org/A006421
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: x^2*(1 + 9*g - 9*g^2 - 20*g^3 + 20*g^4)/((1 - g)^5*(1 -
 *            2*g)^8), g = x*C(x), C the Catalan g.f. (A000108). Since x =
 *            g*(1-g), Lagrange inversion gives a(n) = [t^m]
 *            P(t)/((1-t)^(m+6)*(1-2t)^7), m = n-2, P(t) =
 *            1+9t-9t^2-20t^3+20t^4, i.e. a(n) = S(m) + 9*S(m-1) - 9*S(m-2) -
 *            20*S(m-3) + 20*S(m...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 26, u128 n <= 56,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 471 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A006421_S(idx_t m, idx_t r) {
    /* c = binomial(k+6,6)*2^k, start at k = r */
    seq_t c = 1;
    for (idx_t k = 0; k < r; k++)
        c = c * (2 * (7 + k)) / (k + 1);
    seq_t D = 1; /* binomial(m+5+j, j) */
    seq_t s = 0;
    for (idx_t j = 0; j <= r; j++) {
        s += c * D;
        if (j < r) {
            idx_t k = r - j;
            c = c * k / (2 * (k + 6));
            D = D * (m + 6 + j) / (j + 1);
        }
    }
    return s;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;
    idx_t m = n - 2;
    seq_t pos = A006421_S(m, m);
    seq_t neg = 0;
    if (m >= 1) pos += 9 * A006421_S(m, m - 1);
    if (m >= 2) neg += 9 * A006421_S(m, m - 2);
    if (m >= 3) neg += 20 * A006421_S(m, m - 3);
    if (m >= 4) pos += 20 * A006421_S(m, m - 4);
    return pos - neg;
}
