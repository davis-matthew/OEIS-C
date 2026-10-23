/*
 * A040056  Continued fraction for sqrt(65).
 * https://oeis.org/A040056
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 8; a(n) = 16 for n >= 1 (continued fraction coefficients
 *            of sqrt(65))
 * Range:     u8 n <= 9967, u16 n <= 9967, u32 n <= 9967, u64 n <= 9967,
 *            u128 n <= 9967, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return (n == 0) ? (seq_t)8 : (seq_t)16;
}
