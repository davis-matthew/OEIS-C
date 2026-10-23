/*
 * A040506  Continued fraction for sqrt(530).
 * https://oeis.org/A040506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0)=23, a(n)=46 for n>=1
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return (n == 0) ? (seq_t)23 : (seq_t)46;
}
