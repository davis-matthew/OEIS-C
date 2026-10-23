/*
 * A040059  Continued fraction for sqrt(68).
 * https://oeis.org/A040059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=8; a(n)=4 if n is odd; a(n)=16 if n is even and n>0
 * Range:     u8 n <= 8483, u16 n <= 8483, u32 n <= 8483, u64 n <= 8483,
 *            u128 n <= 8483, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)8;
    return (n & 1) ? (seq_t)4 : (seq_t)16;
}
