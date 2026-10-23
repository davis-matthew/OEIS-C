/*
 * A038794  T(n,n-4), array T as in A038792.
 * https://oeis.org/A038794
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 - 18*n^3 + 143*n^2 - 486*n + 672) / 24 for n >= 7,
 *            with a(4) = 1, a(5) = 5, a(6) = 12
 * Range:     u8 n <= 6, u16 n <= 20, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) return 0;           /* undefined before offset */
    if (n == 4) return 1;
    if (n == 5) return 5;
    if (n == 6) return 12;

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2 */
    seq_t n3 = n2 * nn;          /* n^3 */
    seq_t n4 = n3 * nn;          /* n^4 */

    seq_t term = n4 + 672 + (seq_t)143 * n2;
    term -= (seq_t)18 * n3 + (seq_t)486 * nn;
    return term / 24;
}
