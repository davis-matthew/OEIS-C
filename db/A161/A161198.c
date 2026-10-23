/*
 * A161198  Triangle of polynomial coefficients related to the series
 *          expansions of (1-x)^((-1-2*n)/2).
 * https://oeis.org/A161198
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 27, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A161198: Triangle A161198 flattened row‑by‑row.
   a(n) is the (n+1)-th value in the flattened triangle
   defined by
     a(0,0) = 1
     a(n,0) = (2*n-1)!!
     a(n,n) = 2^n
     a(n,m) = 2*a(n-1,m-1) + (2*n-1)*a(n-1,m)   for 0 < m < n
   where a(n) = a(r,c) for the r,c such that
        sum_{i=0}^{r-1} (i+1) ≤ n < sum_{i=0}^{r} (i+1)  and
        c = n - r*(r+1)/2.
 */
#include <stdlib.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Determine row r and column c in the flattened triangle. */
    idx_t r = 0;
    while ( ((r + 1) * (r + 2)) / 2 <= n )  /* (r+1)(r+2)/2 = total up to row r */
        ++r;
    idx_t c = n - r * (r + 1) / 2;

    /* Special case row 0. */
    if (r == 0) {
        return 1;       /* a(0) = 1 */
    }

    /* Allocate and build rows up to r. */
    seq_t *prev = (seq_t *)malloc(sizeof(seq_t) * 1);
    prev[0] = 1;              /* row 0 */

    for (idx_t cur_row = 1; cur_row <= r; ++cur_row) {
        size_t cur_len = (size_t)cur_row + 1;
        seq_t *cur = (seq_t *)malloc(cur_len * sizeof(seq_t));

        for (idx_t j = 0; j <= cur_row; ++j) {
            if (j == cur_row) {
                /* a(cur_row, cur_row) = 2^cur_row */
                cur[j] = pow2(cur_row);
            } else {
                seq_t term1 = 0;
                if (j > 0) {
                    /* 2 * a(cur_row-1, j-1) */
                    term1 = prev[j - 1] * 2;
                }

                /* (2*cur_row-1) * a(cur_row-1, j) */
                seq_t term2 = prev[j] * (seq_t)(2 * cur_row - 1);

                cur[j] = term1 + term2;
            }
        }

        free(prev);
        prev = cur;
    }

    /* prev now points to row r. */
    seq_t result = prev[c];
    free(prev);
    return result;
}
