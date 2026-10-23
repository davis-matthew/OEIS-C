/*
 * A188555  Number of 4 X n binary arrays without the pattern 0 1 diagonally,
 *          vertically, antidiagonally or horizontally.
 * https://oeis.org/A188555
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1. For n ≥ 1, a(n) = (n^4 - 2*n^3 + 23*n^2 + 26*n + 72) /
 *            24
 * Range:     u8 n <= 8, u16 n <= 35, u32 n <= 567, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    __int128 nn = ( __int128 )n;
    __int128 n2 = nn * nn;          /* n^2   */
    __int128 n3 = n2 * nn;          /* n^3   */
    __int128 n4 = n3 * nn;          /* n^4   */
    __int128 val = n4 - (__int128)2 * n3
                 + (__int128)23 * n2
                 + (__int128)26 * nn
                 + (__int128)72;
    return (seq_t)(val / 24);
}
