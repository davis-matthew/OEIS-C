/*
 * A189384  a(n) = n + [n*r/s] + [n*t/s]; r=1, s=1/sqrt(2), t=1/sqrt(3).
 * https://oeis.org/A189384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*sqrt(2)) + floor(n*sqrt(2/3))
 * Range:     u8 n <= 5, u16 n <= 83, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute n^2 using the wider idx_t type to avoid overflow. */
    idx_t n_sq = n * n;
    /* temp = 2*n^2 ; both powers are done in idx_t. */
    idx_t temp = (idx_t)2 * n_sq;

    /* floor(n*sqrt(2))  = floor(sqrt(2*n^2))    */
    seq_t term1 = floor_sqrt((seq_t)temp);

    /* floor(n*sqrt(2/3))= floor(sqrt((2*n^2)/3))*/
    seq_t term2 = floor_sqrt((seq_t)(temp / (idx_t)3));

    return (seq_t)n + term1 + term2;
}
