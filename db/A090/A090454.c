/*
 * A090454  Fourth column (m=5) of array A090452.
 * https://oeis.org/A090454
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+9)*(n+6)*(n+2)*(n+1)*(n^2+15*n+20)/144
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
/* idx_t and seq_t are provided by the build system; no need to define them. */

seq_t a(idx_t n)
{
    /* Use a larger integer type for intermediate computation */
    unsigned __int128 nn = n;

    unsigned __int128 t1 = nn + (unsigned __int128)9;
    unsigned __int128 t2 = nn + (unsigned __int128)6;
    unsigned __int128 t3 = nn + (unsigned __int128)2;
    unsigned __int128 t4 = nn + (unsigned __int128)1;
    unsigned __int128 t5 = nn * nn + (unsigned __int128)15 * nn + (unsigned __int128)20;

    unsigned __int128 prod = t1 * t2;
    prod *= t3;
    prod *= t4;
    prod *= t5;
    prod /= (unsigned __int128)144;

    return (seq_t)prod;
}
