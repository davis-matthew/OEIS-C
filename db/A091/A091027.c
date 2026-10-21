/*
 * A091027  Sixth column (m=7) of array A090452.
 * https://oeis.org/A091027
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            ((n+12)*(n+9)*(n+3)*(n+2)*(n+1)*(n^5+48*n^4+769*n^3+4782*n^2+11200*n+8400))/86400
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 21, u64 n <= 255, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use unsigned __int128 to hold all intermediate values safely for n ≤ ~10 000. */
    unsigned __int128 nn = (unsigned __int128)n;

    /* First part: (n+12)*(n+9)*(n+3)*(n+2)*(n+1) */
    unsigned __int128 f1 = nn + 12;
    unsigned __int128 f2 = nn + 9;
    unsigned __int128 f3 = nn + 3;
    unsigned __int128 f4 = nn + 2;
    unsigned __int128 f5 = nn + 1;
    unsigned __int128 prod = f1 * f2 * f3 * f4 * f5;

    /* Polynomial part: n^5 + 48 n^4 + 769 n^3 + 4782 n^2 + 11200 n + 8400 */
    unsigned __int128 n2 = nn * nn;          /* n^2 */
    unsigned __int128 n3 = n2 * nn;          /* n^3 */
    unsigned __int128 n4 = n3 * nn;          /* n^4 */
    unsigned __int128 n5 = n4 * nn;          /* n^5 */
    unsigned __int128 poly = n5
                            + (unsigned __int128)48  * n4
                            + (unsigned __int128)769  * n3
                            + (unsigned __int128)4782 * n2
                            + (unsigned __int128)11200 * nn
                            + 8400;

    /* Full numerator and division by 86400 (exact) */
    unsigned __int128 num     = prod * poly;
    unsigned __int128 result  = num / 86400;

    return (seq_t)result;
}
