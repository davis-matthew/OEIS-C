/*
 * A056119  a(n) = n*(n+13)/2.
 * https://oeis.org/A056119
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+13)/2 (offset 0, a(0)=0)
 * Range:     u8 n <= 17, u16 n <= 355, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return sn * (sn + (seq_t)13) / (seq_t)2;
}
