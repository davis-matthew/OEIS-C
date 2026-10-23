/*
 * A158775  a(n) = 1600*n^2 + 40.
 * https://oeis.org/A158775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1600*n^2 + 40
 * Range:     u8 none, u16 n <= 6, u32 n <= 1638, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t once to avoid repeated casts */
    seq_t nn = (seq_t)n;
    /* Compute 1600 * n^2 + 40 */
    return (seq_t)1600 * nn * nn + (seq_t)40;
}
