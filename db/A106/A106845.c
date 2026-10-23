/*
 * A106845  a(n) = n^2 * (n^3 + 2n^2 + 7n - 2) / 8.
 * https://oeis.org/A106845
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^2 * (n^3 + 2n^2 + 7n - 2) / 8
 * Range:     u8 n <= 4, u16 n <= 13, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 nn2 = nn * nn;          /* n^2 */
    unsigned __int128 nn3 = nn2 * nn;         /* n^3 */
    unsigned __int128 nn4 = nn2 * nn2;        /* n^4 */
    unsigned __int128 nn5 = nn4 * nn;         /* n^5 */
    unsigned __int128 numerator = nn5 + 2 * nn4 + 7 * nn3 - 2 * nn2;
    numerator /= 8;
    return (seq_t)numerator;
}
