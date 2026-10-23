/*
 * A022172  Triangle of Gaussian binomial coefficients [ n,k ] for q = 8.
 * https://oeis.org/A022172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 30, u64 n <= 58, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A022172: Triangle of Gaussian binomial coefficients [n,k] for q = 8.
   The flattened sequence a(n) is obtained by row‑major traversal of the
   triangle defined by
     T(0,0) = 1;
     T(n,0) = T(n,n) = 1;
     T(n,k) = T(n-1,k-1) + 8^k * T(n-1,k)   for 1 ≤ k ≤ n-1.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Determine the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        r++;

    /* Column index within that row */
    idx_t c = n - (r * (r + 1)) / 2;

    if (r == 0)        /* row 0 has the single entry 1 */
        return (seq_t)1;

    /* Allocate two temporary rows of size r+1 */
    size_t sz = (size_t)(r + 1);
    seq_t *prev = (seq_t *)malloc(sz * sizeof(seq_t));
    seq_t *cur  = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!prev || !cur) {
        free(prev);
        free(cur);
        return (seq_t)0;   /* allocation failed – value not used */
    }

    /* Row 0 */
    prev[0] = (seq_t)1;

    /* Build rows 1 … r */
    for (idx_t row = 1; row <= r; ++row) {
        cur[0] = (seq_t)1;                 /* left‑hand edge */
        idx_t kmax = row - 1;
        seq_t pow8 = (seq_t)8;              /* 8^1 for k = 1 */
        for (idx_t k = 1; k < row; ++k) {
            cur[k] = prev[k - 1] + pow8 * prev[k];
            pow8 *= (seq_t)8;                /* advance 8^k */
        }
        cur[row] = (seq_t)1;                 /* right‑hand edge */

        /* Swap rows for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur  = tmp;
    }

    seq_t result = prev[c];
    free(prev);
    free(cur);
    return result;
}
