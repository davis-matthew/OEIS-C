/*
 * A006976  Coefficients of Chebyshev T polynomials: a(n) = A053120(n+12, n),
 *          n >= 0.
 * https://oeis.org/A006976
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n*binomial(n+5, 5)*(n+12)/12
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 16, u64 n <= 41, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t c = 1;
    idx_t i;
    /* binomial(n+5,5) computed multiplicatively (each step exact) */
    for (i = 1; i <= 5; i++) {
        c = c * ((seq_t)n + (seq_t)i) / (seq_t)i;
    }
    /* binomial(n+5,5)*(n+12) is always divisible by 3 */
    seq_t t = c * ((seq_t)n + 12) / 3;
    if (n < 2) {
        idx_t k;
        for (k = 0; k < n; k++) t = t * 2;
        return t / 4;
    }
    for (i = 0; i < n - 2; i++) t = t * 2;
    return t;
}
