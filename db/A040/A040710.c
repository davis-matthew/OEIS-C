/*
 * A040710  Continued fraction for sqrt(738).
 * https://oeis.org/A040710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=27, a(n)=6 if n odd else 54 for n>=1
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)27;
    return (n & 1) ? (seq_t)6 : (seq_t)54;
}
