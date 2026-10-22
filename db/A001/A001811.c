/*
 * A001811  Coefficients of Laguerre polynomials.
 * https://oeis.org/A001811
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) * n^2 / (n-4) for n >= 5, a(4) = 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 5; i <= n; i++) {
        result = result * (seq_t)i * (seq_t)i / (seq_t)(i - 4);
    }
    return result;
}
