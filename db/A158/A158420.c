/*
 * A158420  a(n) = 1024*n^2 - 2*n.
 * https://oeis.org/A158420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1024*n^2 - 2*n
 * Range:     u8 none, u16 n <= 8, u32 n <= 2048, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return sn * sn * 1024 - (seq_t)2 * sn;
}
