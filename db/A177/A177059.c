/*
 * A177059  a(n) = 25*n^2 + 25*n + 6.
 * https://oeis.org/A177059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 25*n^2 + 25*n + 6
 * Range:     u8 n <= 2, u16 n <= 50, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* a(n) = 25 * n * (n + 1) + 6 */
    return 25 * nn * (nn + 1) + 6;
}
