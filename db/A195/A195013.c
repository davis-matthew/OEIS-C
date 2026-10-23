/*
 * A195013  Multiples of 2 and of 3 interleaved: a(2n-1) = 2n, a(2n) = 3n.
 * https://oeis.org/A195013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = if n is odd: n+1 (which equals 2*((n+1)/2)) * if n is
 *            even: 3*n/2 (which equals 3*(n/2)) * * This implements the OEIS
 *            definition a(2n-1)=2n , a(2n)=3n
 * Range:     u8 n <= 171, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 by the problem statement */
    if (n & 1) {
        /* odd n: a(n) = n + 1 */
        return (seq_t)n + 1;
    } else {
        /* even n: a(n) = 3*n/2 */
        return ((seq_t)n * 3) / 2;
    }
}
