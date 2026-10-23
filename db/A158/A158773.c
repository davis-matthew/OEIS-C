/*
 * A158773  a(n) = 1600*n^2 - 40.
 * https://oeis.org/A158773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1600*n^2 - 40
 * Range:     u8 none, u16 n <= 6, u32 n <= 1638, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return sn * sn * (seq_t)1600 - (seq_t)40;
}
