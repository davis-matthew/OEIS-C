/*
 * A040042  Continued fraction for sqrt(50) = 5*sqrt(2).
 * https://oeis.org/A040042
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=7; a(n)=14 for n>=1
 * Range:     u8 n <= 8854, u16 n <= 8854, u32 n <= 8854, u64 n <= 8854,
 *            u128 n <= 8854, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    return (n == 0) ? 7 : 14;
}
