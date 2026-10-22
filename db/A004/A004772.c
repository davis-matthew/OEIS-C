/*
 * A004772  Numbers that are not congruent to 1 (mod 4).
 * https://oeis.org/A004772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((4*n - 2) / 3)
 * Range:     u8 n <= 192, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n;
    return (4 * m - 2) / 3;
}
