/*
 * A055806  a(n) = T(n,n-6), array T as in A055801.
 * https://oeis.org/A055806
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (48915 -58884*n +29723*n^2 -7200*n^3 +965*n^4 -66*n^5
 *            +2*n^6 + 3*(-1)^n*(-231345 +98988*n -18505*n^2 +1840*n^3 -95*n^4
 *            +2*n^5))/92160, n > 6
 * Range:     u8 n <= 19, u16 n <= 43, u32 n <= 247, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n < 6) return 0;          /* sequence undefined below the offset */
    if (n == 6) return (seq_t)1;  /* a(6) = 1 */

    __int128 nn = (__int128)n;
    __int128 n2 = nn * nn;
    __int128 n3 = n2 * nn;
    __int128 n4 = n3 * nn;
    __int128 n5 = n4 * nn;
    __int128 n6 = n5 * nn;

    /* main polynomial part */
    __int128 main = ( (__int128)2 * n6
                    - (__int128)66 * n5
                    + (__int128)965 * n4
                    - (__int128)7200 * n3
                    + (__int128)29723 * n2
                    - (__int128)58884 * nn
                    + (__int128)48915 );

    /* alternating part */
    __int128 alt_sign = (n & 1ULL) ? -(__int128)1 : (__int128)1;
    __int128 inner = -(__int128)231345
                     + (__int128)98988 * nn
                     - (__int128)18505 * n2
                     + (__int128)1840 * n3
                     - (__int128)95 * n4
                     + (__int128)2 * n5;
    __int128 alt = (__int128)3 * alt_sign * inner;

    __int128 total = main + alt;

    /* division by 92160 gives the exact integer value */
    seq_t res = (seq_t)(total / 92160);
    return res;
}
