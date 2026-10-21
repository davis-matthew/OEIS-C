/*
 * A051890  a(n) = 2*(n^2 - n + 1).
 * https://oeis.org/A051890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n^2 - n + 1)
 * Range:     u8 n <= 11, u16 n <= 181, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n^2 */
    seq_t sq = (seq_t)n * (seq_t)n;
    /* n^2 - n + 1 */
    sq = sq - (seq_t)n + (seq_t)1;
    /* times 2 */
    return sq * (seq_t)2;
}
