/*
 * A197603  a(n) = floor((n+1/n)^4).
 * https://oeis.org/A197603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n + 1/n)^4). For n>2 this equals n^4 + 4*n^2 + 6;
 *            a(1) = 16, a(2) = 39
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)16;
    if (n == 2) return (seq_t)39;

    seq_t nn = (seq_t)n;          /* cast once to seq_t */
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n4 = n2 * n2;           /* n^4 */

    return n4 + (seq_t)4 * n2 + (seq_t)6;
}
