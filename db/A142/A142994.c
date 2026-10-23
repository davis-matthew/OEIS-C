/*
 * A142994  Crystal ball sequence for the lattice C_5.
 * https://oeis.org/A142994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n + 1)*(32*n^4 + 64*n^3 + 88*n^2 + 56*n + 15)/15
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 62, u64 n <= 5334,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return the precomputed values for the first six terms */
    static const seq_t base[6] = {1, 51, 501, 2471, 8361, 22363};
    if (n < 6)
        return base[n];

    /* Use 128‑bit arithmetic to avoid overflow while evaluating the
       polynomial.  The result fits into seq_t for all n that fit
       into the type. */
    __int128 nn = ( __int128 )n;
    __int128 n2 = nn * nn;
    __int128 n3 = n2 * nn;
    __int128 n4 = n3 * nn;

    __int128 poly =
        (__int128)32 * n4 +
        (__int128)64 * n3 +
        (__int128)88 * n2 +
        (__int128)56 * nn +
        ( __int128 )15;

    __int128 num = (( __int128 )2 * nn + ( __int128 )1) * poly;
    return (seq_t)(num / ( __int128 )15);
}
