/*
 * A168374  a(n) = 7 * floor(n/2).
 * https://oeis.org/A168374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 7 * floor(n/2)
 * Range:     u8 n <= 73, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                         /* offset starts at 1, but guard for safety */
    seq_t half = (seq_t)(n >> 1);
    return half * 7;
}
