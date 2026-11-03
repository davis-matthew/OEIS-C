/*
 * A000970  Fermat coefficients.
 * https://oeis.org/A000970
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, easy
 * Formula:   coefficient of x^n in x^5(3x^5 + 2x^4 + 4x^3 + 3x^2 + 3x +
 *            1)/((1-x^5)(1-x)^4)
 * Range:     u8 n <= 9, u16 n <= 29, u32 n <= 426, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 53 terms through n = 877 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binomial_3(seq_t n) {
    if (n < 3) return 0;
    return n * (n - 1) * (n - 2) / 6;
}

static seq_t poly_coeff(int i) {
    static const seq_t p[6] = {1, 3, 3, 4, 2, 3};
    return p[i];
}

seq_t a(idx_t n) {
    if (n < 5) return 0;
    
    seq_t result = 0;
    seq_t m = (seq_t)(n - 5);
    
    for (seq_t j = 0; 5 * j <= m; j++) {
        seq_t mk = m - 5 * j;
        seq_t limit = (mk < 5) ? mk : 5;
        for (seq_t i = 0; i <= limit; i++) {
            result += poly_coeff((int)i) * binomial_3(mk - i + 3);
        }
    }
    
    return result;
}
