/*
 * A040032  Continued fraction for sqrt(39).
 * https://oeis.org/A040032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=6, a(n)=4 if n is odd, 12 if n is even (n>0)
 * Range:     u8 n <= 9023, u16 n <= 9023, u32 n <= 9023, u64 n <= 9023,
 *            u128 n <= 9023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)6;
    return (seq_t)((n & 1) ? 4 : 12);
}
