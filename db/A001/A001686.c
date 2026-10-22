/*
 * A001686  From a continued fraction.
 * https://oeis.org/A001686
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = c(n)*a(n-1) + a(n-2), a(-1) = 0, a(0) = 1, where c(n) =
 *            A001684(n): c(0..2) = 1, c(n) = c(n-1)*p(n-3) for n >= 3, and
 *            p(n) = c(n)*p(n-1) + p(n-2), p(-2) = p(-1) = 1, p(0) = 1 (the
 *            numerators of the continued fraction)
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t c = 1;
    seq_t pm3 = 1, pm2 = 1, pm1 = 1; /* p(i-3), p(i-2), p(i-1) at loop start */
    seq_t qm2 = 0, qm1 = 1;          /* q(i-2), q(i-1) */
    for (idx_t i = 1; i <= n; i++) {
        if (i >= 3) c *= pm3;
        seq_t p = c * pm1 + pm2;
        seq_t q = c * qm1 + qm2;
        pm3 = pm2;
        pm2 = pm1;
        pm1 = p;
        qm2 = qm1;
        qm1 = q;
    }
    return qm1;
}
