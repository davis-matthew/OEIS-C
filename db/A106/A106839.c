/*
 * A106839  Numbers congruent to 11 mod 16.
 * https://oeis.org/A106839
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*n + 11
 * Range:     u8 n <= 15, u16 n <= 4095, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 69 terms through n = 9298 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return (seq_t)n * (seq_t)16 + (seq_t)11;
}
