/*
 * A099173  Array, A(k,n), read by diagonals: g.f. of k-th row
 *          x/(1-2*x-(k-1)*x^2).
 * https://oeis.org/A099173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the value A(row,col) read by anti‑diagonals of the
 *            square array A(r,k); row = d - offset, col = offset where d =
 *            ⌊(√(8*n+1)-1)/2⌋ and offset = n - d(d+1)/2. The array values
 *            satisfy A(r,0)=0, A(r,1)=1, A(r,k)=2*A(r,k-1)+(r-1)*A(r,k-2) for
 *            k≥2
 * Range:     u8 n <= 7, u16 n <= 146, u32 n <= 368, u64 n <= 976,
 *            u128 n <= 1325, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* compute diagonal index d */
    idx_t tmp = (n << 3) + 1;                 /* 8*n + 1 */
    seq_t sq  = floor_sqrt((seq_t)tmp);       /* floor(sqrt(8*n+1)) */
    idx_t d   = ((idx_t)sq - 1) / 2;           /* ⌊(√(8*n+1)-1)/2⌋ */

    /* cumulative number of terms up to diagonal d‑1 */
    idx_t cum_prev = d * (d + 1) / 2;
    idx_t offset   = n - cum_prev;            /* position on diagonal d */
    idx_t row      = d - offset;              /* first argument of A */
    idx_t col      = offset;                  /* second argument of A */

    /* evaluate A(row,col) by recurrence */
    if (col == 0)   return (seq_t)0;
    if (col == 1)   return (seq_t)1;

    seq_t prev2 = (seq_t)0;   /* A(row,0) */
    seq_t prev1 = (seq_t)1;   /* A(row,1) */
    for (idx_t i = 2; i <= col; ++i) {
        seq_t cur;
        if (row == 0) {                 /* (row-1) = -1 */
            cur = prev1 * 2 - prev2;
        } else {
            cur = prev1 * 2 + (seq_t)(row - 1) * prev2;
        }
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
