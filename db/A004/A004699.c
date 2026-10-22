/*
 * A004699  a(n) = floor(Fibonacci(n)/6).
 * https://oeis.org/A004699
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(Fibonacci(n)/6); computed by tracking F(n) = 6*q(n)
 *            + r(n), with q(n) = q(n-1) + q(n-2) + floor((r(n-1)+r(n-2))/6),
 *            r(n) = (r(n-1)+r(n-2)) mod 6, q(0) = 0, r(0) = 0, q(1) = 0, r(1)
 *            = 1; a(n) = q(n)
 * Range:     u8 n <= 16, u16 n <= 28, u32 n <= 51, u64 n <= 97, u128 n <= 189,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t qp = 0, qc = 0;
    int rp = 0, rc = 1;
    idx_t i;
    if (n == 0) return 0;
    for (i = 1; i < n; i++) {
        int s = rp + rc;
        seq_t qn = qp + qc;
        qn += s / 6;
        qp = qc;
        rp = rc;
        qc = qn;
        rc = s % 6;
    }
    return qc;
}
