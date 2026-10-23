/*
 * A154575  a(n) = 2*n^2 + 12*n + 4.
 * https://oeis.org/A154575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2 + 12*n + 4
 * Range:     u8 n <= 8, u16 n <= 178, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return sn * (sn * 2 + 12) + 4;
}
