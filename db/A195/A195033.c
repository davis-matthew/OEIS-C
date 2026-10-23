/*
 * A195033  Multiples of 21 and of 20 interleaved: a(2n-1) = 21n, a(2n) = 20n.
 * https://oeis.org/A195033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 21*((n+1)/2) when n is odd, a(n) = 20*(n/2) when n is
 *            even
 * Range:     u8 n <= 24, u16 n <= 6240, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1u) == 0u)          /* even n */
    {
        return (seq_t)20 * (seq_t)(n >> 1);
    }
    else                         /* odd n */
    {
        return (seq_t)21 * (seq_t)((n >> 1) + 1);
    }
}
