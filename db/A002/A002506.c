/*
 * A002506  Denominators of coefficients of even powers of x of the expansion
 *          of Bessel function J_2(x).
 * https://oeis.org/A002506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n*(n+2)*a(n-1), a(0) = 8
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = (seq_t)8;
    for (idx_t i = 1; i <= n; i++) {
        result = result * ((seq_t)4 * (seq_t)i * ((seq_t)i + (seq_t)2));
    }
    return result;
}
