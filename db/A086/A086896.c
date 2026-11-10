/*
 * A086896  a(n) = (1/4!)*(A000522(4*n) + 6*A000522(2*n) + 8*A000522(n) + 9).
 * https://oeis.org/A086896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/4!)*(A000522(4*n) + 6*A000522(2*n) + 8*A000522(n) +
 *            9), where A000522(m) = m*A000522(m-1) + 1, A000522(0) = 1
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 5, u128 n <= 8,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 97 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t A000522_a(idx_t n) {
    seq_t result = 1;
    for (idx_t i = 1; i <= n; i++) {
        result = (seq_t)i * result + 1;
    }
    return result;
}

seq_t a(idx_t n) {
    seq_t big = A000522_a(4 * n);
    seq_t rest = (seq_t)6 * A000522_a(2 * n) + (seq_t)8 * A000522_a(n) + 9;
    seq_t q = big / 24;
    seq_t r = big % 24;
    return q + (r + rest) / 24;
}
