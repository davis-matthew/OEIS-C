/*
 * A215814  a(n) = 60516*n^2 - 61008*n + 2481403.
 * https://oeis.org/A215814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 60516*n^2 - 61008*n + 2481403
 * Range:     u8 none, u16 none, u32 n <= 266, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t un = (seq_t)n;
    return 60516 * un * un - 61008 * un + 2481403;
}
