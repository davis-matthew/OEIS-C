/*
 * A040155  Continued fraction for sqrt(168).
 * https://oeis.org/A040155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=12; for n>=1, a(n)=1 if n is odd, else 24
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)12;
    }
    return (n & 1) ? (seq_t)1 : (seq_t)24;
}
