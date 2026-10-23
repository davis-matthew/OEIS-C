/*
 * A157266  a(n) = 1728*n - 408.
 * https://oeis.org/A157266
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1728*n - 408
 * Range:     u8 none, u16 n <= 38, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to seq_t before performing the arithmetic in seq_t */
    seq_t m = (seq_t)n;
    return 1728 * m - 408;
}
