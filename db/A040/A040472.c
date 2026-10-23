/*
 * A040472  Continued fraction for sqrt(495).
 * https://oeis.org/A040472
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0) = 22; a(n) = 4 if n is odd, otherwise 44, for n >= 1
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)22;
    }
    return (n & 1) ? (seq_t)4 : (seq_t)44;
}
