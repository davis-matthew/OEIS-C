/*
 * A049704  Array T read by antidiagonals; T(i,j)=number of nonnegative slopes
 *          of lines determined by two points in the triangular set {(x,y):
 *          0<=x<=i, 0<=y<=j-j*x/i} of lattice points.
 * https://oeis.org/A049704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of distinct nonnegative slopes of lines determined
 *            by two points in the triangular set {(x,y): 0 <= x <= i, 0 <= y
 *            <= j - j*x/i} read by antidiagonals. Implementation uses the
 *            derived counting: horizontal slope always present for i>0; for
 *            i==0 the only slope is counted once. All oth...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   antidiagonal_coords
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/antidiagonal_coords.h"

/* Euclidean GCD for idx_t. */
static idx_t gcd_idx(idx_t a, idx_t b)
{
    while (b != 0) {
        idx_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);

    /* Special case when i == 0 */
    if (i == 0) {
        return (seq_t)(j == 0 ? 0 : 1);
    }

    seq_t count = 1; /* horizontal slope (p = 0, q = 1) */

    /* Count all positive rational slopes p/q with gcd(p,q) == 1 */
    for (idx_t q = 1; q <= i; ++q) {
        /* maximum possible numerator for this denominator */
        idx_t maxP = (idx_t)((seq_t)j * (i - q) / i);
        for (idx_t p = 1; p <= maxP; ++p) {
            if (gcd_idx(p, q) == 1) {
                ++count;
            }
        }
    }

    return count;
}
