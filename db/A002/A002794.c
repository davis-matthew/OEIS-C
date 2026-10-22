/*
 * A002794  Numerators of convergents to Lehmer's constant.
 * https://oeis.org/A002794
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, nice
 * Formula:   a(n) = d(n)*a(n-1) + a(n-2), a(0) = 1, a(1) = 1, where the d(k)
 *            are the partial quotients of Lehmer's constant xi = [0; 1, d(1),
 *            d(2), ...]: d(1) = 1, d(k) = (c(k-1)^2 + 1) / d(k-1), with c(1)
 *            = 1 and c(k) = c(k-1)^2 + c(k-1) + 1 (A002065), because
 *            d(k-1)*d(k) = c(k-1)^2 + 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t a0 = (seq_t)1, a1 = (seq_t)1;   /* a(k-2), a(k-1) */
    seq_t dprev = (seq_t)1;               /* d(k-1) */
    seq_t c = (seq_t)1;                   /* c(k-1) */
    idx_t k;
    if (n < 2) return (seq_t)1;
    for (k = 2; k <= n; k++) {
        seq_t dk = (c * c + 1) / dprev;
        seq_t ak = dk * a1 + a0;
        a0 = a1;
        a1 = ak;
        if (k < n) {
            c = c * c + c + 1;
            dprev = dk;
        }
    }
    return a1;
}
