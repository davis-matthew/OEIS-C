/*
 * A017343  a(n) = (10*n + 6)^3.
 * https://oeis.org/A017343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 6)^3
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 161, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t base = (seq_t)n * 10 + 6;
    return base * base * base;
}
