/*
 * A022846  Nearest integer to n*sqrt(2).
 * https://oeis.org/A022846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(n*sqrt(2)) = (floor_sqrt(8*n*n)+1)/2
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* compute 8*n*n using an intermediate 128‑bit type to avoid overflow
       on 64‑bit seq_t builds */
    __int128 t = (__int128)n;
    t = t * t * 8;          /* t = 8*n*n */
    seq_t x = (seq_t)t;     /* convert to the build’s integer type */
    seq_t s = floor_sqrt(x);/* floor(sqrt(x)) */
    return (s + 1) / 2;     /* round to nearest integer */
}
