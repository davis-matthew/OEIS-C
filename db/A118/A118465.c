/*
 * A118465  a(n) = 8*n^3 + n.
 * https://oeis.org/A118465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n^3 + n
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 812, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t Sn = (seq_t)n;
    return 8 * Sn * Sn * Sn + Sn;
}
