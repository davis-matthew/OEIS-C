/*
 * A057979  a(n) = 1 for even n and (n-1)/2 for odd n.
 * https://oeis.org/A057979
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 1 if n is even; (n-1)/2 if n is odd
 * Range:     u8 n <= 334, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0)
        return (seq_t)1;
    else
        return (((seq_t)n - (seq_t)1) / (seq_t)2);
}
