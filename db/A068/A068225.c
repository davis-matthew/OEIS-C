/*
 * A068225  Neighbor in 1-2 direction of numbers arranged as clockwise spiral.
 * https://oeis.org/A068225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 225, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   int_sqrt, n_to_xy, xy_to_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence by determining the coordinates of n in the
   Ulam spiral and returning the number immediately to the right
   (the 1‑2 direction). */

#include <stdint.h>
#include "../utils/int_sqrt.h"
#include "../utils/n_to_xy.h"
#include "../utils/xy_to_n.h"

/* Main sequence function */
seq_t a(idx_t n)
{
    int64_t x, y;
    n_to_xy(n, &x, &y);

    /* neighbor to the right: (x+1, y) */
    int64_t nx = x + 1;
    int64_t ny = y;

    return (seq_t)xy_to_n(nx, ny);
}
