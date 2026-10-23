/*
 * A156772  a(n) = 729*n - 198.
 * https://oeis.org/A156772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 729*n - 198
 * Range:     u8 none, u16 n <= 90, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 729*n - 198 with seq_t arithmetic. */
    return (seq_t)n * (seq_t)729 - (seq_t)198;
}
