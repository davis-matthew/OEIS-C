/*
 * A098848  a(n) = n*(n + 14).
 * https://oeis.org/A098848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n + 14) (offset 0)
 * Range:     u8 n <= 10, u16 n <= 249, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;      /* convert index to sequence type */
    return sn * (sn + (seq_t)14);
}
