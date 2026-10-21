/*
 * A083381  Square array giving the number of trellis edges T(i,j) (i >= 0, j
 *          >= 0), read by antidiagonals.
 * https://oeis.org/A083381
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   T(i,j) = 3*i*j + i + j a(n) = T(i,j) where (i,j) are the
 *            coordinates obtained by reading the square array by
 *            antidiagonals (row-major in each antidiagonal)
 * Range:     u8 n <= 7, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"   /* declares seq_t floor_sqrt(seq_t) */

seq_t a(idx_t n)
{
    /* Compute s = floor((sqrt(8*n + 1) - 1) / 2)  using seq_t arithmetic
       to avoid overflow. */
    seq_t D  = (seq_t)8 * (seq_t)n + 1;
    seq_t sqrtD = floor_sqrt(D);
    idx_t s = (idx_t)((sqrtD - 1) / 2);

    /* triangular number tri = s*(s+1)/2  (seq_t) */
    seq_t tri = (seq_t)s * (s + 1) / 2;

    /* offset within diagonal */
    seq_t t_seq = (seq_t)n - tri;   /* seq_t holds the difference */
    idx_t t = (idx_t)t_seq;        /* conversion safe for all n that fit in idx_t */

    idx_t i = s - t;
    idx_t j = t;

    return (seq_t)3 * (seq_t)i * (seq_t)j + (seq_t)i + (seq_t)j;
}
