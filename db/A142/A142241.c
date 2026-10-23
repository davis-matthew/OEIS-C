/*
 * A142241  a(n) = 24*n + 14.
 * https://oeis.org/A142241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = 24*n + 14
 * Range:     u8 n <= 10, u16 n <= 2730, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return (seq_t)24 * (seq_t)n + (seq_t)14;
}
