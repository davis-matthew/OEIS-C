/*
 * A145447  a(n) = the smallest square (and/or cube) > n-th prime.
 * https://oeis.org/A145447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min( ceil(sqrt(pₙ))² , ceil(cuberoot(pₙ))³ ) where pₙ is
 *            the n‑th prime
 * Range:     u8 n <= 18, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   floor_cube_root, floor_sqrt, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/floor_sqrt.h"
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n)
{
    /* Get the n‑th prime.  get_prime(1) = 2, get_prime(2) = 3, ...  */
    uint32_t prim = get_prime(n);
    seq_t p = (seq_t)prim;

    /* Next square greater than p.  If p is a perfect square,
       floor_sqrt(p) returns its root; adding 1 gives the next root. */
    seq_t s = floor_sqrt(p);
    seq_t s1 = s + (seq_t)1;
    seq_t next_sq = s1 * s1;

    /* Next cube greater than p.  Similar logic. */
    seq_t c = floor_cube_root(p);
    seq_t c1 = c + (seq_t)1;
    seq_t next_cube = c1 * c1 * c1;

    return (next_sq < next_cube) ? next_sq : next_cube;
}
