/*
 * A008361  Coordination sequence for D_8 lattice.
 * https://oeis.org/A008361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 16/315 * n * (219 + 1036*n^2 + 826*n^4 + 124*n^6)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 18, u64 n <= 434, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    unsigned __int128 nn  = (unsigned __int128)n;
    unsigned __int128 n2  = nn * nn;
    unsigned __int128 n4  = n2 * n2;
    unsigned __int128 n6  = n4 * n2;

    unsigned __int128 expr =
        (unsigned __int128)219
        + (unsigned __int128)1036 * n2
        + (unsigned __int128)826 * n4
        + (unsigned __int128)124 * n6;

    unsigned __int128 val = (unsigned __int128)16 * nn * expr;
    val /= (unsigned __int128)315;

    return (seq_t)val;
}
