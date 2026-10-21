/*
 * A065014  Duplicate of A013945.
 * https://oeis.org/A065014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = coefficient of x^(n-1) in
 *            (x^5+3x^4-x^3+2x^2+2x+3)/(1-x^2)^3
 * Range:     i8 n <= 12, i16 n <= 52, i32 n <= 52, i64 n <= 52, i128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    idx_t T = n - 1;                       /* T = n-1 */
    const int coeffs[6] = {3, 2, 2, -1, 3, 1}; /* coefficients for x^0..x^5 */
    for (idx_t j = 0; j <= 5; ++j) {
        if (T < j) continue;
        if (((T - j) & 1) != 0) continue; /* only even difference contributes */
        idx_t k = (T - j) >> 1;            /* k = (T-j)/2 */
        seq_t kk = (seq_t)k;
        seq_t binom = (kk + 2) * (kk + 1) / 2; /* (k+2 choose 2) */
        seq_t term = (seq_t)coeffs[j] * binom;
        result += term;
    }
    return result;
}
