/*
 * A166943  One third of product plus sum of six consecutive nonnegative
 *          numbers.
 * https://oeis.org/A166943
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 + 15n^5 + 85n^4 + 225n^3 + 274n^2 + 126n + 15)/3
 * Range:     u8 n <= 0, u16 n <= 4, u32 n <= 44, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n3 = n2 * nn;           /* n^3 */
    seq_t n4 = n3 * nn;           /* n^4 */
    seq_t n5 = n4 * nn;           /* n^5 */
    seq_t n6 = n5 * nn;           /* n^6 */

    seq_t num = n6 + 15 * n5 + 85 * n4 + 225 * n3 + 274 * n2 + 126 * nn + 15;
    return num / 3;
}
