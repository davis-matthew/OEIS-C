/*
 * A147875  Second heptagonal numbers: a(n) = n*(5*n+3)/2.
 * https://oeis.org/A147875
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(5*n+3)/2
 * Range:     u8 n <= 9, u16 n <= 161, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return sn * ((seq_t)5 * sn + (seq_t)3) / (seq_t)2;
}
