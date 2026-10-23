/*
 * A161704  a(n) = (3*n^5 - 35*n^4 + 145*n^3 - 235*n^2 + 152*n + 30)/30.
 * https://oeis.org/A161704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n^5 - 35*n^4 + 145*n^3 - 235*n^2 + 152*n + 30)/30
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 136, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Use a 128‑bit signed integer for intermediate arithmetic.
       The polynomial is exact for all n, and the result is always an integer.  */
    __int128 N = ( __int128) (unsigned long long)n;
    __int128 val =
          (__int128)3 * N * N * N * N * N
        - (__int128)35 * N * N * N * N
        + (__int128)145 * N * N * N
        - (__int128)235 * N * N
        + (__int128)152 * N
        + (__int128)30;
    val /= 30;
    return (seq_t)val;
}
