/*
 * A190705  a(n) = 6*n^2*(2*n + 1).
 * https://oeis.org/A190705
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n^2*(2*n + 1)
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 709, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute 6 * n * n * (2*n + 1) safely in seq_t */
    return (seq_t)6 * nn * nn * (2 * nn + 1);
}
