/*
 * A040063  Continued fraction for sqrt(72).
 * https://oeis.org/A040063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=8; for n>=1, a(n)=2 if n is odd else 16 (period 2: 2,16)
 * Range:     u8 n <= 9906, u16 n <= 9906, u32 n <= 9906, u64 n <= 9906,
 *            u128 n <= 9906, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)8;
    return ((n & 1ULL) ? (seq_t)2 : (seq_t)16);
}
