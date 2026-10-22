/*
 * A001093  a(n) = n^3 + 1.
 * https://oeis.org/A001093
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^3 + 1
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 1625, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return (seq_t)(n * n * n + 1);
}
