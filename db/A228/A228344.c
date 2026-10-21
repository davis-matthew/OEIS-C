/*
 * A228344  a(n) = floor(3*n^2/4) - 1.
 * https://oeis.org/A228344
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(3*n^2/4) - 1
 * Range:     u8 n <= 18, u16 n <= 295, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* Compute floor(3*n^2/4) - 1 */
    return nn * nn * 3 / 4 - 1;
}
