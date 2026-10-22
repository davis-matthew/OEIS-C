/*
 * A005568  Product of two successive Catalan numbers C(n)*C(n+1).
 * https://oeis.org/A005568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = C(n)*C(n+1), where C(0) = 1 and C(k+1) =
 *            2*(2*k+1)*C(k)/(k+2) (Catalan numbers)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t c = 1;
    idx_t k;
    for (k = 0; k < n; k++) {
        c = c * (seq_t)(2 * (2 * k + 1));
        c = c / (seq_t)(k + 2);
    }
    seq_t c1 = c * (seq_t)(2 * (2 * n + 1));
    c1 = c1 / (seq_t)(n + 2);
    return c * c1;
}
