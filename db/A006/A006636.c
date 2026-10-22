/*
 * A006636  a(n) = (n + 1)*(n + 2)*(n + 4)*(n + 8)*(n + 15)/120.
 * https://oeis.org/A006636
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 1)*(n + 2)*(n + 4)*(n + 8)*(n + 15)/120
 * Range:     u8 n <= 3, u16 n <= 18, u32 n <= 85, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t m = (seq_t)n;
    return (m + 1) * (m + 2) * (m + 4) * (m + 8) * (m + 15) / 120;
}
