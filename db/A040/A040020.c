/*
 * A040020  Continued fraction for sqrt(26).
 * https://oeis.org/A040020
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0) = 5; a(n) = 10 for n >= 1
 * Range:     u8 n <= 9886, u16 n <= 9886, u32 n <= 9886, u64 n <= 9886,
 *            u128 n <= 9886, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n == 0) ? (seq_t)5 : (seq_t)10;
}
