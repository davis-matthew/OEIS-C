/*
 * A040273  Continued fraction for sqrt(291).
 * https://oeis.org/A040273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=17; for n>0, a(n)=17 if n is odd, a(n)=34 if n is even
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)17;
    if (n & 1)  return (seq_t)17;
    return (seq_t)34;
}
