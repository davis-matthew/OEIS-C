/*
 * A217947  a(n) = (n+1)*(n^3+15*n^2+74*n+132)/12.
 * https://oeis.org/A217947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n^3+15*n^2+74*n+132)/12
 * Range:     u8 n <= 0, u16 n <= 12, u32 n <= 258, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n1 = nn + 1;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t inside = n3 + 15 * n2 + 74 * nn + (seq_t)132;
    seq_t num = n1 * inside;
    return num / 12;
}
