/*
 * A040043  Continued fraction for sqrt(51).
 * https://oeis.org/A040043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(n) = (n == 0 || n % 2 == 1) ? 7 : 14
 * Range:     u8 n <= 9911, u16 n <= 9911, u32 n <= 9911, u64 n <= 9911,
 *            u128 n <= 9911, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return (n == 0 || (n & 1)) ? (seq_t)7 : (seq_t)14;
}
