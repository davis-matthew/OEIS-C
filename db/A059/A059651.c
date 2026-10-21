/*
 * A059651  a(n) = [[(k^2)*n]-(k*[k*n])], where k = cube root of 2 and [] is
 *          the floor function.
 * https://oeis.org/A059651
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 119, i16 n <= 119, i32 n <= 119, i64 n <= 119,
 *            i128 n <= 119, big any n
 * Helpers:   int_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A059651
   Formula: a(n) = floor(k²·n) – ceil(k·floor(k·n)),  k = 2^(1/3)
   (Equivalently a(n) = b – c – 1 for n>0, where
      b = floor((4·n³)^(1/3)) = floor(k²·n)
      m = floor((2·n³)^(1/3)) = floor(k·n)
      c = floor((2·m³)^(1/3)) = floor(k·m)
   and a(0)=0.) */

#include <stdint.h>
#include "../utils/int_cube_root.h"

seq_t a(idx_t n)
{
    /* Handle offset separately; m=0 gives k*m=0 and ceil(0)=0 */
    if (n == 0)
        return (seq_t)0;

    /* Compute n³ in 128‑bit to avoid overflow */
    __int128 n3 = (__int128)n * n * n;

    /* floor(k·n) = floor((2·n³)^(1/3)) */
    uint64_t two_n3   = (uint64_t)(2 * n3);
    uint64_t m        = int_cube_root(two_n3);

    /* floor(k²·n) = floor((4·n³)^(1/3)) */
    uint64_t four_n3  = (uint64_t)(4 * n3);
    uint64_t b        = int_cube_root(four_n3);

    /* floor(k·m) = floor((2·m³)^(1/3)) */
    __int128 m3      = (__int128)m * m * m;
    uint64_t two_m3   = (uint64_t)(2 * m3);
    uint64_t c        = int_cube_root(two_m3);

    /* a(n) = b – (c + 1)  (since m > 0 for n > 0) */
    seq_t result = (seq_t)((seq_t)b - (seq_t)c - 1);
    return result;
}
