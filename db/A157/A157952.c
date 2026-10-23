/*
 * A157952  a(n) = 162*n + 1.
 * https://oeis.org/A157952
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 162*n + 1
 * Range:     u8 n <= 1, u16 n <= 404, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* for safety handle n == 0, although the offset is 1 */
    if (n == 0) return 0;
    /* compute the value using seq_t arithmetic only */
    return (seq_t)n * (seq_t)162 + (seq_t)1;
}
