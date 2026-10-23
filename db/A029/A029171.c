/*
 * A029171  Expansion of 1/((1-x^2)*(1-x^4)*(1-x^5)*(1-x^6)).
 * https://oeis.org/A029171
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3+33*n^2+324*n+1440 - 15*(n^2+17*n+38)*(n mod
 *            2))/1440)
 * Range:     u8 n <= 61, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    __int128 nn = ( __int128 ) n;
    __int128 val = nn * nn * nn
                 + 33 * nn * nn
                 + 324 * nn
                 + 1440;
    if ( n & 1U ) {
        __int128 adjust = 15 * ( nn * nn + 17 * nn + 38 );
        val -= adjust;
    }
    return ( seq_t )( val / 1440 );
}
