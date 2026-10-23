/*
 * A125201  a(n) = 8*n^2 - 7*n + 1.
 * https://oeis.org/A125201
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n^2 - 7*n + 1
 * Range:     u8 n <= 6, u16 n <= 90, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t sn = (seq_t)n;
    return (seq_t)8 * sn * sn - (seq_t)7 * sn + (seq_t)1;
}
