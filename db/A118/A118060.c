/*
 * A118060  a(n) = 1681*n^2 - 984*n - 696.
 * https://oeis.org/A118060
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = 1681*n^2 - 984*n - 696
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return sn * sn * 1681 - sn * 984 - 696;
}
