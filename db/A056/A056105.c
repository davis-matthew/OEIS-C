/*
 * A056105  First spoke of a hexagonal spiral.
 * https://oeis.org/A056105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*n^2 - 2*n + 1
 * Range:     u8 n <= 9, u16 n <= 148, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return 3 * sn * sn - 2 * sn + 1;
}
