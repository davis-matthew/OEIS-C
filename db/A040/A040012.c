/*
 * A040012  Continued fraction for sqrt(17).
 * https://oeis.org/A040012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=4, a(n)=8 for n >= 1
 * Range:     u8 n <= 9338, u16 n <= 9338, u32 n <= 9338, u64 n <= 9338,
 *            u128 n <= 9338, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n == 0) ? (seq_t)4 : (seq_t)8;
}
