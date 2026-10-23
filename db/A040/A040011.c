/*
 * A040011  Continued fraction for sqrt(15).
 * https://oeis.org/A040011
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=3; for n>=1, a(n)=1 if n is odd, 6 if n is even
 * Range:     u8 n <= 9007, u16 n <= 9007, u32 n <= 9007, u64 n <= 9007,
 *            u128 n <= 9007, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)3;
    return (n & (seq_t)1) ? (seq_t)1 : (seq_t)6;
}
