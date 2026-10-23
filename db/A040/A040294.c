/*
 * A040294  Continued fraction for sqrt(312).
 * https://oeis.org/A040294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(0)=17; a(n) = 34 when n % 4 == 0, otherwise 1. (Multiplicative
 *            rule: a(2)=1, a(2^e)=34 for e≥2, a(p^e)=1 for odd primes.)
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)17;
    if ((n & 3) == 0)   /* n % 4 == 0 */
        return (seq_t)34;
    return (seq_t)1;
}
