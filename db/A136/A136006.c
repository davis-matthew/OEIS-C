/*
 * A136006  a(n) = n^6 - n^3.
 * https://oeis.org/A136006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^6 - n^3
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;        /* convert index to sequence type */
    seq_t n2 = sn * sn;          /* n^2 */
    seq_t n3 = n2 * sn;          /* n^3 */
    seq_t n6 = n3 * n3;          /* n^6 */
    return n6 - n3;              /* n^6 - n^3 */
}
