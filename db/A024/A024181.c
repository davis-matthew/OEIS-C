/*
 * A024181  Integer part of ((4th elementary symmetric function of
 *          2,3,...,n+4)/(2nd elementary symmetric function of 2,3,...,n+4)).
 * https://oeis.org/A024181
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 39, u32 n <= 670, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A024181
   a(n) = floor(1/240 * n*(n+1)*(15 n^4 + 330 n^3 + 2765 n^2 + 10482 n + 15208)/(3 n^2 + 35 n + 104))
*/
#include <stdint.h>

seq_t a(idx_t n)
{
    /* cast index to an unsigned 64‑bit integer for intermediate calculations */
    uint64_t nn = (uint64_t)n;
    /* all heavy‑weight calculations are performed in 128 bit to avoid overflow */
    __int128 const pp = (__int128)15*nn*nn*nn*nn
                      + (__int128)330*nn*nn*nn
                      + (__int128)2765*nn*nn
                      + (__int128)10482*nn
                      + (__int128)15208;
    __int128 numer = (__int128)nn * (nn + 1) * pp;
    __int128 denom = (__int128)240 * ( (__int128)3*nn*nn + (__int128)35*nn + 104 );
    /* integer division implements the floor */
    return (seq_t)(numer / denom);
}
