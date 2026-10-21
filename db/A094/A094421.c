/*
 * A094421  a(n) = n * (6*n^2 + 6*n + 1).
 * https://oeis.org/A094421
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (6*n^2 + 6*n + 1)
 * Range:     u8 n <= 3, u16 n <= 21, u32 n <= 894, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;               /* convert index to sequence type */
    seq_t factor = 6*sn*sn + 6*sn + 1; /* 6*n^2 + 6*n + 1   */
    return sn * factor;               /* n * factor         */
}
