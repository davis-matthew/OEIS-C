/*
 * A008493  Expansion of (1-x^11) / (1-x)^11.
 * https://oeis.org/A008493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*n*(193248 + 152900*n^2 + 16401*n^4 + 330*n^6 +
 *            n^8)/362880 for n>0, a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 36, u64 n <= 439, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    unsigned __int128 nn = n;
    unsigned __int128 n2 = nn * nn;     /* n^2 */
    unsigned __int128 n4 = n2 * n2;     /* n^4 */
    unsigned __int128 n6 = n4 * n2;     /* n^6 */
    unsigned __int128 n8 = n4 * n4;     /* n^8 */

    unsigned __int128 term =
        (unsigned __int128)193248
        + (unsigned __int128)152900 * n2
        + (unsigned __int128)16401  * n4
        + (unsigned __int128) 330   * n6
        + n8;

    unsigned __int128 num = (unsigned __int128)11 * nn * term;
    unsigned __int128 res = num / 362880; /* exact division */

    return (seq_t)res;
}
