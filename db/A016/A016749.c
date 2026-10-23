/*
 * A016749  a(n) = (2*n)^9.
 * https://oeis.org/A016749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n)^9
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 5, u64 n <= 69, u128 n <= 9556,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n * 2;
    seq_t res = 1;
    for (int i = 0; i < 9; ++i) {
        res *= x;
    }
    return res;
}
