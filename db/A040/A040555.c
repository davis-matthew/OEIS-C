/*
 * A040555  Continued fraction for sqrt(580).
 * https://oeis.org/A040555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0)=24; for n>0, a(n)=12 if n is odd, 48 if n is even
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)24;
    }
    return (n & 1) ? (seq_t)12 : (seq_t)48;
}
