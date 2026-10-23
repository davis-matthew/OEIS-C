/*
 * A196506  a(n) = 1*3*5 + 3*5*7 + 5*7*9 + ... (n terms).
 * https://oeis.org/A196506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^4 + 8*n^3 + 7*n^2 - 2*n
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 214, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;        /* n^2 */
    seq_t n3 = n2 * nn;        /* n^3 */
    seq_t n4 = n3 * nn;        /* n^4 */
    return 2 * n4 + 8 * n3 + 7 * n2 - 2 * nn;
}
