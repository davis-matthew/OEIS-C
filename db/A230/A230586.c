/*
 * A230586  a(n) = n^5 - 5*n^3 + 5*n.
 * https://oeis.org/A230586
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 - 5*n^3 + 5*n
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 84, u64 n <= 7131,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* n */
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n3 = n2 * nn;           /* n^3 */
    seq_t n5 = n3 * n2;           /* n^5 = n^3 * n^2 */
    seq_t res = n5 - (seq_t)5 * n3 + (seq_t)5 * nn;
    return res;
}
