/*
 * A010875  a(n) = n mod 6.
 * https://oeis.org/A010875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n mod 6
 * Range:     u8 n <= 65538, u16 n <= 65538, u32 n <= 65538, u64 n <= 65538,
 *            u128 n <= 65538, big any n
 * Validated: standard, pass, 125 terms through n = 9576 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Return the remainder of n divided by 6 as a seq_t */
    return (seq_t)(n % 6);
}
