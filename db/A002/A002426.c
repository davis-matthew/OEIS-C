/*
 * A002426  Central trinomial coefficients: largest coefficient of (1 + x +
 *          x^2)^n.
 * https://oeis.org/A002426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, core, easy
 * Formula:   a(n) = ((2*n - 1)*a(n-1) + 3*(n - 1)*a(n-2))/n, a(0) = a(1) = 1
 *            (evaluated as quotient/remainder parts so intermediates stay <=
 *            a(n))
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t p2 = 1, p1 = 1;
    idx_t i;
    if (n <= 1) return 1;
    for (i = 2; i <= n; i++) {
        seq_t N = (seq_t)i;
        seq_t q1, r1, q2, r2, c1, c2, cur;
        if (N == 0) break; /* only possible for tiny fixed widths, far past fitting terms */
        q1 = p1 / N; r1 = p1 % N;
        q2 = p2 / N; r2 = p2 % N;
        c1 = N * 2 - 1;
        c2 = (N - 1) * 3;
        cur = c1 * q1 + c2 * q2 + (c1 * r1 + c2 * r2) / N;
        p2 = p1;
        p1 = cur;
    }
    return p1;
}
