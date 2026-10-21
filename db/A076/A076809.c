/*
 * A076809  a(n) = n^4 + 853n^3 + 2636n^2 + 3536n + 1753.
 * https://oeis.org/A076809
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^4 + 853*n^3 + 2636*n^2 + 3536*n + 1753
 * Range:     u8 none, u16 n <= 3, u32 n <= 160, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn   = (seq_t)n;          /* n */
    seq_t n2   = nn * nn;           /* n^2 */
    seq_t n3   = n2 * nn;           /* n^3 */
    seq_t n4   = n3 * nn;           /* n^4 */
    seq_t term = n4
               + (seq_t)853 * n3
               + (seq_t)2636 * n2
               + (seq_t)3536 * nn
               + (seq_t)1753;
    return term;
}
