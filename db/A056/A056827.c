/*
 * A056827  a(n) = floor(n^2/6).
 * https://oeis.org/A056827
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^2/6)
 * Range:     u8 n <= 15, u16 n <= 369, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast n to seq_t first to avoid truncation in the multiplication   */
    seq_t sq = (seq_t)n * (seq_t)n;
    return sq / (seq_t)6;
}
