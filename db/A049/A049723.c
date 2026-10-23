/*
 * A049723  Array T read by diagonals; T(i,j)=number of lattice points (x,y)
 *          in closed right half-plane and circle with radius (0,0)-to-(i,j).
 * https://oeis.org/A049723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   * a(n) = Σ_{y = 0}^{⌊√(i² + j²)⌋} ( 2·⌊√(i² + j² − y²)⌋ + 1 ), *
 *            where (i,j) are obtained from n by reading the array T by
 *            diagonals: * let d = ⌊(√(8n+1) − 1)/2⌋, * k = n − d(d+1)/2, * i
 *            = k, * j = d − k. * T(i,j) counts the integer lattice points
 *            (x,y) with y ≥ 0 lying inside * or on the circle...
 * Range:     u8 n <= 35, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   _floor_isqrt_u64, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Find diagonal d containing index n */
    idx_t t  = (idx_t)8 * n + 1;
    idx_t m  = _floor_isqrt_u64(t);          /* floor(sqrt(8n+1)) */
    idx_t d  = (m - 1) / 2;                  /* diagonal number */

    /* Offset of n within diagonal d */
    idx_t prev_total = d * (d + 1) / 2;       /* total terms before diagonal d */
    idx_t k          = n - prev_total;       /* 0 ≤ k ≤ d */
    idx_t i          = k;
    idx_t j          = d - k;

    /* Radius squared of the circle */
    seq_t r2 = (seq_t)i * (seq_t)i + (seq_t)j * (seq_t)j;

    /* Count lattice points with y ≥ 0 inside the circle */
    seq_t r   = floor_sqrt(r2);              /* floor(sqrt(r2)) */
    seq_t sum = 0;
    for (seq_t y = 0; y <= r; ++y)
    {
        seq_t rem     = r2 - y * y;          /* remaining square radius */
        seq_t x_max   = floor_sqrt(rem);    /* maximum |x| for this y */
        sum += (seq_t)2 * x_max + 1;        /* points with both signs + origin */
    }
    return sum;
}
