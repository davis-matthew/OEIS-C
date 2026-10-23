/*
 * A116767  Number of permutations of length n which avoid the patterns 1234,
 *          3142, 3421.
 * https://oeis.org/A116767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^6 + 45*n^5 - 245*n^4 - 465*n^3 + 8164*n^2 - 24780*n +
 *            25920)/720 for n>=4 a(1)=1, a(2)=2, a(3)=6, a(4)=21
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)6;
    if (n == 4) return (seq_t)21;

    /* use signed 128‑bit for intermediate arithmetic to avoid unsigned underflow */
    __int128 N   = (__int128)n;
    __int128 N2  = N * N;
    __int128 N3  = N2 * N;
    __int128 N4  = N3 * N;
    __int128 N5  = N4 * N;
    __int128 N6  = N5 * N;

    __int128 num = N6
                 + (__int128)45 * N5
                 - (__int128)245 * N4
                 - (__int128)465 * N3
                 + (__int128)8164 * N2
                 - (__int128)24780 * N
                 + (__int128)25920;

    return (seq_t)(num / (__int128)720);
}
