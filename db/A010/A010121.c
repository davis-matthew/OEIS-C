/*
 * A010121  Continued fraction for sqrt(7).
 * https://oeis.org/A010121
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=2; for n>0, a(n)=4 if n%4==0 else a(n)=1
 * Range:     u8 n <= 9852, u16 n <= 9852, u32 n <= 9852, u64 n <= 9852,
 *            u128 n <= 9852, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;
    if ((n & 3) == 0)     /* n%4==0 for unsigned */
        return (seq_t)4;
    return (seq_t)1;
}
