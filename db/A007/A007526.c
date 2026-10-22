/*
 * A007526  a(n) = n*(a(n-1) + 1), a(0) = 0.
 * https://oeis.org/A007526
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(a(n-1) + 1), a(0) = 0
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    for (idx_t i = 1; i <= n; i++) {
        result = (seq_t)i * (result + 1);
    }
    return result;
}
