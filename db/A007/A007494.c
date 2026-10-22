/*
 * A007494  Numbers that are congruent to 0 or 2 mod 3.
 * https://oeis.org/A007494
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*floor(n/2) + 2*(n mod 2)
 * Range:     u8 n <= 170, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return 3 * ((seq_t)(n / 2)) + 2 * (n % 2);
}
