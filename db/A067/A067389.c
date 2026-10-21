/*
 * A067389  a(n) = 3*n^3 + 2*n^2 + n.
 * https://oeis.org/A067389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^3 + 2*n^2 + n
 * Range:     u8 n <= 4, u16 n <= 27, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;         /* convert index to seq_t */
    seq_t n2 = sn * sn;          /* n^2 */
    seq_t n3 = n2 * sn;          /* n^3 */
    return 3 * n3 + 2 * n2 + sn; /* 3*n^3 + 2*n^2 + n */
}
