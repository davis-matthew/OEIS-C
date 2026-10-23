/*
 * A132756  a(n) = n*(n + 27)/2.
 * https://oeis.org/A132756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+27)/2
 * Range:     u8 n <= 12, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = n;               /* convert index to seq_t */
    return sn * (sn + 27) / 2;  /* compute n*(n+27)/2 exactly */
}
