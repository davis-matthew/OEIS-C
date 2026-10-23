/*
 * A036560  StirlingS2[ n,m ] triangle summed down the columns.
 * https://oeis.org/A036560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 57, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A036560: a(n) = 0 for n = 0.  For n ≥ 1,
   let r be the row (0‑based) containing n in the triangle
   of Stirling numbers of the second kind summed column‑wise.
   If n is the j‑th element in row r (0‑based), then
      a(n) = r                              , j = 0
      a(n) = Σ_{i = j+1}^{r+1} S(i, j+1)   , j ≥ 1
   where S(i,k) are Stirling numbers of the second kind.
   The triangle rows are: {0},
   {1,1},
   {2,4,1},
   {3,11,7,1},
   {4,26,32,11,1}, … */

#include <stddef.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free.h"

static inline idx_t triangular(idx_t k) { return k * (k + 1) / 2; }

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* Find the row r such that T(r) ≤ n < T(r+1), where T(k)=k(k+1)/2 */
    idx_t r = 0;
    while (triangular(r + 1) <= n) r++;

    /* Position of n in its row: 0‑based */
    idx_t pos = n - triangular(r);

    if (pos == 0) return (seq_t)r;   /* first element of the row */

    idx_t m   = pos + 1;             /* column number (1‑based) */
    idx_t rows = r + 1;              /* highest row index needed */

    /* Allocate temporary arrays */
    seq_t *prev = seq_alloc_new_array(rows + 1);
    seq_t *cur  = seq_alloc_new_array(rows + 1);
    seq_t *col  = seq_alloc_new_array(rows + 1);

    /* Initialise to zero */
    for (size_t i = 0; i < rows + 1; ++i) {
        prev[i] = 0;
        cur[i]  = 0;
        col[i]  = 0;
    }

    /* Compute Stirling numbers row by row and accumulate column sums */
    for (idx_t i = 1; i <= rows; ++i) {
        for (idx_t k = 1; k <= i; ++k) {
            seq_t val;
            if (k == i) {
                val = 1;                     /* S(i,i) = 1 */
            } else {
                val = prev[k - 1] + (seq_t)k * prev[k];
            }
            cur[k] = val;
            col[k] += val;
        }
        /* Prepare for next iteration */
        seq_t *tmp = prev; prev = cur; cur = tmp;
    }

    seq_t result = col[m];

    /* Free temporary arrays */
    seq_free(prev);
    seq_free(cur);
    seq_free(col);

    return result;
}
