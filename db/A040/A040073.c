/*
 * A040073  Continued fraction for sqrt(83).
 * https://oeis.org/A040073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 9; for n >= 1: a(n) = 9 if n is odd, else 18
 * Range:     u8 n <= 9927, u16 n <= 9927, u32 n <= 9927, u64 n <= 9927,
 *            u128 n <= 9927, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)9;
    return (n & (idx_t)1) ? (seq_t)9 : (seq_t)18;
}
