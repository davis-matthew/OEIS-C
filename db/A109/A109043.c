/*
 * A109043  a(n) = lcm(n,2).
 * https://oeis.org/A109043
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = lcm(n,2)
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    if ((n & 1) == 0)
        return (seq_t)n;                /* even n: lcm(n,2) = n */
    else
        return ((seq_t)n) << 1;         /* odd n:  lcm(n,2) = 2*n */
}
