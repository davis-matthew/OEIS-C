/*
 * A000346  a(n) = 2^(2*n+1) - binomial(2*n+1, n+1).
 * https://oeis.org/A000346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) + C(n), a(0) = 1, where C(n) = A000108(n) is the
 *            Catalan number, C(n) = C(n-1)*(4n-2)/(n+1), C(0) = 1 (from G.f.
 *            c(x)/(1-4x))
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t c = (seq_t)1;
    seq_t r = (seq_t)1;
    idx_t k;
    for (k = 1; k <= n; k++) {
        c = c * (seq_t)(4 * k - 2) / (seq_t)(k + 1);
        r = r * 4 + c;
    }
    return r;
}
