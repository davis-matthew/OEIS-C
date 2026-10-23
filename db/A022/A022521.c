/*
 * A022521  a(n) = (n+1)^5 - n^5.
 * https://oeis.org/A022521
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = (n+1)^5 - n^5
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 170, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;            /* n^2 */
    seq_t n3 = n2 * nn;            /* n^3 */
    seq_t n4 = n3 * nn;            /* n^4 */
    seq_t result = 5 * n4          /* 5 n^4 */
                + 10 * n3          /* 10 n^3 */
                + 10 * n2          /* 10 n^2 */
                + 5 * nn           /* 5 n   */
                + (seq_t)1;        /* + 1   */
    return result;
}
