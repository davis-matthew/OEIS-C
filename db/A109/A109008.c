/*
 * A109008  a(n) = gcd(n,4).
 * https://oeis.org/A109008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = gcd(n,4)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n & 3 == 0   → n is a multiple of 4, gcd = 4
       n & 1 == 0   → n is even but not a multiple of 4, gcd = 2
       otherwise   → n is odd, gcd = 1   */
    if ((n & 3ULL) == 0)
        return (seq_t)4;
    if ((n & 1ULL) == 0)
        return (seq_t)2;
    return (seq_t)1;
}
