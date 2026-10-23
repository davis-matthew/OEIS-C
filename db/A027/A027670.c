/*
 * A027670  Number of different bracelets with 6 beads of at most n colors,
 *          allowing turning over.
 * https://oeis.org/A027670
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 60, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A027670: Number of different bracelets with 6 beads of at most n colors,
   allowing turning over.

   Formula: a(n) = n*(n+1)*(n^4 - n^3 + 4*n^2 + 2)/12
            = (n^6 + 3*n^4 + 4*n^3 + 2*n^2 + 2*n) / 12
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    /* cast to unsigned __int128 to keep intermediate products safe */
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 n2 = nn * nn;          /* n^2 */
    unsigned __int128 n3 = n2 * nn;          /* n^3 */
    unsigned __int128 n4 = n3 * nn;          /* n^4 */
    unsigned __int128 n6 = n3 * n3;          /* n^6 */

    unsigned __int128 num = n6
                           + 3 * n4
                           + 4 * n3
                           + 2 * n2
                           + 2 * nn;

    return (seq_t)(num / 12);
}
