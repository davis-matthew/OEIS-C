/*
 * A082111  a(n) = n^2 + 5*n + 1.
 * https://oeis.org/A082111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2 + 5*n + 1
 * Range:     u8 n <= 13, u16 n <= 253, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* convert index to sequence type */
    seq_t res = nn * nn;          /* n^2 */
    res += 5 * nn;                /* + 5*n */
    res += 1;                     /* + 1 */
    return res;
}
