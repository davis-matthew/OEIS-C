/*
 * A160050  Numerator of the Harary number for the star graph s_n.
 * https://oeis.org/A160050
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator of (1/4)*(n+2)*(n-1)
 * Range:     u8 n <= 22, u16 n <= 362, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a wider integer type to avoid overflow for small seq_t sizes */
    unsigned __int128 prod = (unsigned __int128)(n + 2) * (unsigned __int128)(n - 1);

    unsigned __int128 g;
    if (prod % 4 == 0)
        g = 4;
    else if (prod % 2 == 0)
        g = 2;
    else
        g = 1;

    return (seq_t)(prod / g);
}
