/*
 * A004523  Two even followed by one odd; or floor(2n/3).
 * https://oeis.org/A004523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2*n/3)
 * Range:     u8 n <= 383, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return (seq_t)(2 * (n / 3) + (2 * (n % 3)) / 3);
}
