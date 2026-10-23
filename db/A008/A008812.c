/*
 * A008812  Expansion of (1+x^5)/((1-x)^2*(1-x^5)).
 * https://oeis.org/A008812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-3)*floor((n+1)/5) - 5*floor((n+1)/5)^2 + n + 1
 * Range:     u8 n <= 34, u16 n <= 571, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute the n-th term of OEIS A008812.
 * The formula is valid for all n >= 0.
 * Uses a signed 128-bit intermediate to avoid overflow for large n.
 */
seq_t a(idx_t n)
{
    idx_t k = (n + 1) / 5;                         /* floor((n+1)/5) */
    __int128 mult = (__int128)2 * (__int128)n - 3; /* 2*n - 3 */
    __int128 part1 = mult * (__int128)k;           /* (2*n-3)*k */
    __int128 part2 = (__int128)5 * (__int128)k * (__int128)k; /* 5*k^2 */
    __int128 result = part1 - part2 + (__int128)n + 1;
    return (seq_t)result;
}
