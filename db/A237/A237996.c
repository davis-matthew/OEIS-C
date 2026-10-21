/*
 * A237996  Triangular array read by rows. T(n,k) is the number of even
 *          permutations of {1,2,...,n} that have exactly k cycles, n >= 0, 0
 *          <= k <= n.
 * https://oeis.org/A237996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 47, u32 n <= 106, u64 n <= 254,
 *            u128 n <= 632, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* a(n) = entry of the triangle read by rows.  
   The triangle entry T(r,k) is the number of even permutations of
   {1,…,r} having exactly k cycles.  It satisfies
        T(r,k) = 0                 if r and k have different parity,
        T(r,k) = |s(r,k)|          otherwise, where |s(r,k)|
        is the unsigned Stirling number of the first kind.  */

/* External integer types defined by the build system. */
seq_t a(idx_t n)
{
    /* 1. Locate the row r and column k from the flattened index n. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t k = n - r * (r + 1) / 2;

    /* 2. Cache of Stirling numbers, built incrementally. */
    static idx_t max_row = 0;      /* highest row computed so far */
    static size_t rows_cap = 0;    /* current capacity of the rows array */
    static seq_t **rows = NULL;    /* rows[i][j] holds |s(i,j)| */

    /* First‑time initialization of the cache (row 0). */
    if (rows == NULL) {
        rows = (seq_t **)malloc(sizeof(seq_t *));
        rows_cap = 1;
        rows[0] = (seq_t *)malloc(sizeof(seq_t));
        rows[0][0] = 1;           /* |s(0,0)| = 1  */
        max_row = 0;
    }

    /* 3. Grow the cache if the requested row r is not yet computed. */
    if (r > max_row) {
        /* Ensure the rows array has enough capacity. */
        size_t new_cap = rows_cap ? rows_cap : 1;
        while (new_cap <= (size_t)r + 1)
            new_cap <<= 1;
        seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
        rows = tmp;
        rows_cap = new_cap;

        /* Compute new rows from max_row+1 up to r. */
        for (idx_t rr = max_row + 1; rr <= r; ++rr) {
            rows[rr] = (seq_t *)malloc((size_t)(rr + 1) * sizeof(seq_t));
            for (idx_t kk = 0; kk <= rr; ++kk)
                rows[rr][kk] = 0;
            if (rr == 0) {          /* base case already set above */
                rows[0][0] = 1;
            } else {
                for (idx_t kk = 1; kk <= rr; ++kk) {
                    seq_t term1 = rows[rr - 1][kk - 1];
                    seq_t term2 = (kk <= rr - 1)
                                  ? rows[rr - 1][kk] * (seq_t)(rr - 1)
                                  : 0;
                    rows[rr][kk] = term1 + term2;
                }
            }
        }
        max_row = r;
    }

    /* 4. Return the desired entry, respecting the parity rule. */
    if ((r & 1) != (k & 1))
        return 0;
    return rows[r][k];
}
