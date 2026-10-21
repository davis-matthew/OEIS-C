/*
 * A218332  The sequence of coefficients of cubic polynomials p(x-n), where
 *          p(x) = x^3 - 3*x + 1.
 * https://oeis.org/A218332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(4*k) = 1; a(4*k+1) = -3*k; a(4*k+2) = 3*k^2 - 3; a(4*k+3) =
 *            -k^3 + 3*k + 1
 * Range:     i8 n <= 26, i16 n <= 59, i32 n <= 59, i64 n <= 59, i128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t k = n / 4;
    seq_t k_t = (seq_t)k;
    switch (n & 3) {
        case 0: return (seq_t)1;
        case 1: return - (seq_t)3 * k_t;
        case 2: return (seq_t)3 * k_t * k_t - (seq_t)3;
        case 3: return - k_t * k_t * k_t + (seq_t)3 * k_t + (seq_t)1;
    }
    return 0; /* unreachable, but keeps compiler happy */
}
