/*
 * A233273  Bisection of A233272: a(n) = A233272(2n+1).
 * https://oeis.org/A233273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A233272(2n+1), where A233272(m) = 1 + floor(m/2) +
 *            A233272(floor(m/2)), A233272(0) = 1
 * Range:     u8 n <= 126, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Validated: standard, pass, 79 terms through n = 7113 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n * 2 + 1;
    seq_t ans = 1;
    while (m != 0) {
        ans += m / 2 + 1;
        m >>= 1;
    }
    return ans;
}
