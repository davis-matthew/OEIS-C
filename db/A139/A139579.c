/*
 * A139579  a(n) = 2*n^2 + 15*n.
 * https://oeis.org/A139579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*n^2 + 15*n
 * Range:     u8 n <= 8, u16 n <= 177, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return sn * (sn * 2 + 15);
}
