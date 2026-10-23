/*
 * A160828  a(n) = 4*n^4 + 24*n^3 + 84*n^2 + 144*n + 98.
 * https://oeis.org/A160828
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^4 + 24*n^3 + 84*n^2 + 144*n + 98
 * Range:     u8 n <= 0, u16 n <= 9, u32 n <= 179, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;        /* n */
    seq_t n2   = nn * nn;        /* n^2 */
    seq_t n3   = n2 * nn;        /* n^3 */
    seq_t n4   = n3 * nn;        /* n^4 */
    seq_t res  = 4 * n4 + 24 * n3 + 84 * n2 + 144 * nn + 98;
    return res;
}
