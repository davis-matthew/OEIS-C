/*
 * A078404  Expansion of Molien series for a certain 4-D group of order 192.
 * https://oeis.org/A078404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n^3+3*n^2+30*n+81)/72 * - ((n^3+2*n^2+2*n) mod
 *            4)*n/24 * - (n mod 4)/4)
 * Range:     u8 n <= 25, u16 n <= 166, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* reduce modulo 4 early */
    unsigned r = (unsigned)(n & 3U);                 /* n mod 4 */
    unsigned r2 = (r*r*r + 2*r*r + 2*r) & 3U;        /* (n^3+2*n^2+2*n) mod 4   */

    /* compute the numerator in 128‑bit to avoid overflow */
    unsigned __int128 N   = n;
    unsigned __int128 num = N*N*N + 3*N*N + 30*N + 81; /* n^3 + 3n^2 + 30n + 81 */
    num -= 3 * (unsigned __int128)r2 * N;             /* subtract 3*r2*n        */
    num -= 18 * (unsigned __int128)r;                 /* subtract 18*(n mod 4)  */

    /* final division by 72 (floor for non‑negative values) */
    return (seq_t)(num / 72);
}
