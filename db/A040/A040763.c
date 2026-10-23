/*
 * A040763  Continued fraction for sqrt(792).
 * https://oeis.org/A040763
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=28; for n>0, a(n)=7 if n is odd, otherwise 56
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)28;
    return (seq_t)((n & 1) ? 7 : 56);
}
