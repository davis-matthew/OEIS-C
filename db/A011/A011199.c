/*
 * A011199  a(n) = (n+1)*(2*n+1)*(3*n+1).
 * https://oeis.org/A011199
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(2*n+1)*(3*n+1)
 * Range:     u8 n <= 2, u16 n <= 21, u32 n <= 893, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return (sn + 1) * (sn * 2 + 1) * (sn * 3 + 1);
}
