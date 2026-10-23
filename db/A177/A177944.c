/*
 * A177944  Array T(n,m) = 1/Beta(n+1, m+1) - n - m read by antidiagonals.
 * https://oeis.org/A177944
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS formula:  a(n) = (j+1) * binomial(i+j+1, i) - (i + j)
 * where the pair (i, j) is obtained by reading the array T(n,m)
 * along anti‑diagonals.  In particular
 *   d  = floor((sqrt(8*n+1) - 1)/2)          // diagonal index
 *   i  = n - d*(d+1)/2                       // position on the diagonal
 *   j  = d - i
 */
#include <math.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* find the diagonal d for the given index n
     * (triangular numbers: 0,1,3,6,10,...)
     */
    idx_t d = (idx_t)((sqrt((long double)8.0 * n + 1.0) - 1.0) / 2.0);

    /* correct possible rounding errors */
    while ((d + 1) * (d + 2) / 2 <= n) ++d;
    while (d * (d + 1) / 2 > n) --d;

    idx_t before = d * (d + 1) / 2;           /* elements before diag d   */
    idx_t i = n - before;                      /* row index on diag d      */
    idx_t j = d - i;                           /* column index             */

    /* compute T(i,j) = (j+1)*C(i+j+1,i) - (i + j)   */
    seq_t comb = binomial((idx_t)(i + j + 1), (idx_t)i);
    seq_t term = (seq_t)(j + 1) * comb;
    term -= (seq_t)(i + j);
    return term;
}
