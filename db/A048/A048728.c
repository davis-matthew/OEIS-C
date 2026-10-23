/*
 * A048728  Differences between A008585 (multiples of 3) and A048724.
 * https://oeis.org/A048728
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 * (n & (n >> 1))
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the bitwise AND of n and (n >> 1), then multiply by 4 */
    idx_t m = n & (n >> 1);
    return (seq_t)(m << 2);
}
