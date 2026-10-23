/*
 * A108625  Square array, read by antidiagonals, where row n equals the
 *          crystal ball sequence for the A_n lattice.
 * https://oeis.org/A108625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 81, u32 n <= 260, u64 n <= 835,
 *            u128 n <= 3033, big any n
 * Helpers:   binom_small, row_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A108625: Square array read by antidiagonals, row n = crystal ball sequence for A_n lattice.
   Formula implemented:
      Let d = row_of(n)  (i.e. the largest integer with d(d+1)/2 ≤ n)
      Let start = d*(d+1)/2
      Let offset = n - start
      row = d - offset
      col = offset
      Then a(n) = T(row, col) with
           T(r, c) = Σ_{i=0..c} binom(r,i)^2 * binom(r+c-i, c-i)   (OEIS definition)
*/
#include <stdbool.h>
#include "../utils/row_of.h"
#include "../utils/binom_small.h"

seq_t a(idx_t n)
{
    /* Determine antidiagonal (row/col) coordinates */
    idx_t d = row_of(n);                      /* d satisfies d(d+1)/2 ≤ n < (d+1)(d+2)/2 */
    idx_t start = d * (d + 1) / 2;            /* first index of diagonal d */
    idx_t offset = n - start;                 /* position within the diagonal (0…d) */
    idx_t row = d - offset;                   /* row index on this diagonal */
    idx_t col = offset;                       /* column index on this diagonal */

    /* Compute T(row, col) */
    seq_t result = 0;
    for (idx_t i = 0; i <= col; ++i)
    {
        seq_t c1 = binom_small(row, i);
        c1 = c1 * c1;                        /* binom(row,i)^2 */
        seq_t c2 = binom_small(row + col - i, col - i);
        result += c1 * c2;                   /* accumulate term */
    }
    return result;
}
