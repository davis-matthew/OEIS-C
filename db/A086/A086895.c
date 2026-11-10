/*
 * A086895  a(n) = 1/3!*(A000522(3*n) + 3*A000522(n) + 2).
 * https://oeis.org/A086895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A000522(3*n) + 3*A000522(n) + 2)/6, where A000522(m) =
 *            m*A000522(m-1) + 1, A000522(0) = 1. A000522(3n) is tracked as
 *            6*q + r (0 <= r < 6) so q stays about the size of a(n) and
 *            doesn't overflow early
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Validated: standard, pass, 29 terms through n = 98 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t q = 0;
    seq_t r = 1;
    idx_t m = 3 * n;
    for (idx_t i = 1; i <= m; i++) {
        seq_t t = (seq_t)i * r + 1;
        q = (seq_t)i * q + t / 6;
        r = t % 6;
    }
    seq_t s = 1;
    for (idx_t i = 1; i <= n; i++) {
        s = (seq_t)i * s + 1;
    }
    return q + (r + 3 * s + 2) / 6;
}
