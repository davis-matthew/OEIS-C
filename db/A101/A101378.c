/*
 * A101378  a(n) = n^2*(n^3+1)/2.
 * https://oeis.org/A101378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2*(n^3+1)/2
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 96, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t prod = n2 * (n3 + 1);  /* n^2*(n^3+1) */
    return prod / 2;
}
