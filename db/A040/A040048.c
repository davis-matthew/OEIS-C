/*
 * A040048  Continued fraction for sqrt(56).
 * https://oeis.org/A040048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   not stated
 * Range:     u8 n <= 9889, u16 n <= 9889, u32 n <= 9889, u64 n <= 9889,
 *            u128 n <= 9889, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(0)=7; for n>0: a(n)=2 if n is odd; a(n)=14 if n is even */

seq_t a(idx_t n)
{
    if (n == 0)
        return 7;
    return (n & 1) ? 2 : 14;
}
