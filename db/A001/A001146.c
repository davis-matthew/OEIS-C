/*
 * A001146  a(n) = 2^(2^n).
 * https://oeis.org/A001146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, changed
 * Formula:   a(n) = 2^(2^n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = (seq_t)2;
    for (idx_t i = 0; i < n; i++) {
        result = result * result;
    }
    return result;
}
