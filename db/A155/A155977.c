/*
 * A155977  a(n) = n^5 + n^3.
 * https://oeis.org/A155977
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 + n^3
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 84, u64 n <= 7131,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;      /* convert index to value type */
    seq_t n2 = sn * sn;       /* n^2 */
    seq_t n3 = n2 * sn;       /* n^3 */
    seq_t n5 = n3 * n2;       /* n^5 */
    return n5 + n3;           /* n^5 + n^3 */
}
