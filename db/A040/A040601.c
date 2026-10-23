/*
 * A040601  Continued fraction for sqrt(627).
 * https://oeis.org/A040601
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=25, a(n>=1) = (n%2?25:50)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return (n == 0) ? (seq_t)25 : ((n & 1) ? (seq_t)25 : (seq_t)50);
}
