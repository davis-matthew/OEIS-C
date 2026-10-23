/*
 * A040599  Continued fraction for sqrt(624).
 * https://oeis.org/A040599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult, less
 * Formula:   a(0)=24, a(n)=48 if n is even and n>0, a(n)=1 if n is odd
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)24;
    if ((n & 1ULL) == 0) return (seq_t)48;
    return (seq_t)1;
}
