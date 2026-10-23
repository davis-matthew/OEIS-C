/*
 * A108429  Triangle read by rows: T(n,k) is number of paths from (0,0) to
 *          (3n,0) that stay in the first quadrant (but may touch the
 *          horizontal axis), consisting of steps u=(2,1), U=(1,2), or
 *          d=(1,-1) and have k down steps (d).
 * https://oeis.org/A108429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(floor(sqrt(n)), n - floor(sqrt(n))^2)
 * Range:     u8 n <= 20, u16 n <= 57, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   binomial, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Where T(n,k) = binomial(n,2n-k)*binomial(n+k,n-1)/n,   T(0,0)=1       */

#include <math.h>
#include <stdint.h>

/* Provided utility: binomial(idx_t N, idx_t K) → seq_t  */
#include "../utils/binomial.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    idx_t row = floor_sqrt(n);
    idx_t col = n - (unsigned __int128)row * row;

    /* Special case T(0,0) = 1 */
    if (row == 0) return (seq_t)1;

    /* k1 = 2*row - col   (0 ≤ k1 ≤ row) */
    idx_t k1 = 2 * row - col;
    /* k2 = row - 1       (0 ≤ k2 ≤ row-1) */
    idx_t k2 = row - 1;

    seq_t a1 = binomial(row, k1);                   /* binomial(row, 2*row - col) */
    seq_t a2 = binomial(row + col, k2);              /* binomial(row + col, row - 1) */
    seq_t prod = a1 * a2;                            /* intermediate product */
    seq_t result = prod / (seq_t)row;                /* division by row is exact */

    return result;
}
