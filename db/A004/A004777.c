/*
 * A004777  Numbers not congruent to 7 mod 8.
 * https://oeis.org/A004777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n-1)*8/7)
 * Range:     u8 n <= 224, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return ((seq_t)n - 1) * 8 / 7;
}
