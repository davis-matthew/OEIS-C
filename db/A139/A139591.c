/*
 * A139591  A139275(n) followed by 18-gonal number A051870(n+1).
 * https://oeis.org/A139591
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = (4*n*n + n - 3*(n mod 2)) / 2
 * Range:     u8 n <= 7, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = n;
    seq_t numerator = (nn * nn * 4) + nn;   /* 4*n^2 + n */
    if (n & 1)
        numerator -= (seq_t)3;              /* subtract 3 for odd n */
    return numerator / 2;                   /* divide by 2 */
}
