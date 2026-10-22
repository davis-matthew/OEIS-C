/*
 * A003101  a(n) = Sum_{k = 1..n} (n - k + 1)^k.
 * https://oeis.org/A003101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=1..n} (n - k + 1)^k
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 27, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    for (idx_t k = 1; k <= n; k++) {
        seq_t base = (seq_t)(n - k + 1);
        seq_t term = 1;
        for (idx_t i = 0; i < k; i++) {
            term *= base;
        }
        result += term;
    }
    return result;
}
