/*
 * A040873  Continued fraction for sqrt(904).
 * https://oeis.org/A040873
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 30; for n ≥ 1, a(n) = 15 if n is odd, otherwise 60
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0u) {
        return (seq_t)30;
    }
    return (n & 1u) ? (seq_t)15 : (seq_t)60;
}
