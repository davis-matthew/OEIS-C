/*
 * A074989  Distance from n to nearest cube.
 * https://oeis.org/A074989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2452, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cube_root_floor_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Distance from n to nearest cube
   a(n) = 0 if n is a perfect cube, otherwise the minimum of
          n - floor(cuberoot(n))^3  and  (ceil(cuberoot(n)))^3 - n   */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/cube_root_floor_idx.h"

seq_t a(idx_t n)
{
    idx_t k = cube_root_floor_idx(n);
    unsigned __int128 k3   = (unsigned __int128)k * k * k;
    if ((unsigned __int128)n == k3)
        return 0;                        /* n is exactly a cube */

    unsigned __int128 k1_3 = (unsigned __int128)(k + 1) * (k + 1) * (k + 1);
    unsigned __int128 dist1 = (unsigned __int128)n - k3;      /* n - k^3 */
    unsigned __int128 dist2 = k1_3 - (unsigned __int128)n;    /* (k+1)^3 - n */
    unsigned __int128 mindist = dist1 < dist2 ? dist1 : dist2;

    return (seq_t)mindist;                /* safe cast – tests ensure it fits */
}
