/*
 * A040041  Continued fraction for sqrt(48).
 * https://oeis.org/A040041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=6; for n>0: a(n)=12 if n is even, else a(n)=1
 *            (Multiplicative: a(2^e)=12, a(p^e)=1 for odd prime p)
 * Range:     u8 n <= 9702, u16 n <= 9702, u32 n <= 9702, u64 n <= 9702,
 *            u128 n <= 9702, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)               /* a(0) = 6 */
        return (seq_t)6;
    if ((n & (idx_t)1) == 0)  /* n even → 12 */
        return (seq_t)12;
    return (seq_t)1;          /* n odd → 1 */
}
