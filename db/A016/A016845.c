/*
 * A016845  a(n) = (4n+3)^9.
 * https://oeis.org/A016845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4n+3)^9
 * Range:     u8 none, u16 n <= 0, u32 n <= 2, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t base = (seq_t)4 * n + 3;
    seq_t result = 1;
    for (int i = 0; i < 9; ++i) {
        result *= base;
    }
    return result;
}
