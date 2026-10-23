/*
 * A040015  Continued fraction for sqrt(20).
 * https://oeis.org/A040015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy
 * Formula:   a(0)=4; for n>=1, a(n)=2 if n is odd, 8 if n is even
 * Range:     u8 n <= 9940, u16 n <= 9940, u32 n <= 9940, u64 n <= 9940,
 *            u128 n <= 9940, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)4;
    /* n is odd -> 2, even -> 8 */
    return (n & 1) ? (seq_t)2 : (seq_t)8;
}
