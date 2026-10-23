/*
 * A126664  Continued fraction expansion of sqrt(3)/2.
 * https://oeis.org/A126664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  cofr, nonn, easy
 * Formula:   a(0)=0, a(1)=1, a(n)=2*(2+(-1)^n) for n>1
 * Range:     u8 n <= 110, u16 n <= 110, u32 n <= 110, u64 n <= 110,
 *            u128 n <= 110, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return (seq_t)0;
    if (n == (idx_t)1)
        return (seq_t)1;
    return ((n & (idx_t)1) ? (seq_t)2 : (seq_t)6);
}
