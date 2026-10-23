/*
 * A123350  a(n) = (n^4 + 2n^3 + 5n^2 + 4)/4.
 * https://oeis.org/A123350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4 + 2n^3 + 5n^2 + 4)/4
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 337, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* cast index to seq_t */
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n3 = n2 * nn;           /* n^3 */
    seq_t n4 = n3 * nn;           /* n^4 */
    seq_t val = n4 + (seq_t)2 * n3 + (seq_t)5 * n2 + (seq_t)4;
    return val / (seq_t)4;
}
