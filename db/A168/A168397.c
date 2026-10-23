/*
 * A168397  a(n) = 8 * floor(n/2).
 * https://oeis.org/A168397
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 8 * floor(n/2)
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is >= 1; floor division by shifting right one bit */
    return ( (seq_t)n >> 1 ) * 8;
}
