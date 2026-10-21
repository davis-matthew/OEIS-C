/*
 * A214246  Number A(n,k) of compositions of n where differences between
 *          neighboring parts are in {-k,0,k}; square array A(n,k), n>=0,
 *          k>=0, read by antidiagonals.
 * https://oeis.org/A214246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of compositions of the n-coordinate of the
 *            antidiagonal (n,k) with differences between neighboring parts in
 *            {-k,0,k}. The mapping from the linear index n to the coordinate
 *            (n,k) uses the antidiagonal ordering: n0 = offset, k0 = sum -
 *            offset with sum = floor((sqrt(8*n+1)-1)/2)
 * Range:     u8 n <= 102, u16 n <= 322, u32 n <= 1078, u64 n <= 4092,
 *            u128 n <= 8935, big any n
 * Helpers:   count_compositions
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes */
#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include "../utils/count_compositions.h"

/* Helper: integer square root for 64‑bit values */
static inline idx_t int_sqrt_u64(idx_t x)
{
    idx_t r = sqrt((double)x);
    while ((r + 1) * (r + 1) <= x) ++r;
    while (r * r > x) --r;
    return r;
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    /* Map linear index n to antidiagonal coordinate (n0,k0) */
    idx_t d = 8 * n + 1;
    idx_t sqrt_d = int_sqrt_u64(d);
    idx_t sum = (sqrt_d - 1) / 2;            /* floor of (sqrt(8n+1)-1)/2 */

    /* Adjust if necessary due to flooring */
    while ((sum + 1) * (sum + 2) / 2 <= n) ++sum;
    while (sum * (sum + 1) / 2 > n) --sum;

    idx_t offset = n - sum * (sum + 1) / 2;
    idx_t n0 = offset;
    idx_t k0 = sum - offset;

    return count_compositions(n0, k0);
}
