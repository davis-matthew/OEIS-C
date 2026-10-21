/*
 * A078739  Triangle of generalized Stirling numbers S_{2,2}(n,k) read by rows
 *          (n>=1, 2<=k<=2n).
 * https://oeis.org/A078739
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy
 * Formula:   a(n) is the nth term of the flattened triangle S_{2,2}(n,k) read
 *            by rows. The triangle entry T(r,k) for 2 ≤ k ≤ 2r is defined by:
 *            T(1,2) = 1, for r ≥ 2: T(r,k) = T(r-1,k-2) + 2·(k-1)·T(r-1,k-1)
 *            + k·(k-1)·T(r-1,k), with T(r-1,x)=0 if x<2 or x>2(r-1). The
 *            sequence a(n) is the nth element of the str...
 * Range:     u8 n <= 11, u16 n <= 27, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   idx_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/idx_isqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined, but no query for n<1 */

    /* --- compute the row and column of the nth flattened element --- */
    idx_t r = idx_isqrt(n - 1) + 1;          /* row index (1‑based)   */
    idx_t prev_sq = (r - 1) * (r - 1);      /* number of terms before this row */
    idx_t pos = n - prev_sq;                /* position in this row (1‑based)  */
    idx_t k = pos + 1;                      /* column in the triangle (2 … 2r) */

    /* --- static storage of all previously computed rows --- */
    static seq_t **rows = NULL;       /* array of pointers to rows */
    static idx_t max_row = 0;         /* highest row computed so far      */

    /* Create first row if needed */
    if (max_row == 0) {
        rows = (seq_t **)malloc(sizeof(seq_t *) * 2);  /* indices 0 … 1 */
        rows[0] = NULL;
        idx_t size = 2 * 1 + 1;            /* indices 0 … 2 */
        seq_t *row1 = (seq_t *)malloc(sizeof(seq_t) * size);
        memset(row1, 0, sizeof(seq_t) * size);
        row1[2] = (seq_t)1;
        rows[1] = row1;
        max_row = 1;
    }

    /* Compute missing rows up to r */
    while (max_row < r) {
        idx_t new_row = max_row + 1;
        idx_t prev_max = 2 * max_row;          /* last valid index of previous row */
        idx_t new_size = 2 * new_row + 1;
        seq_t *curr = (seq_t *)malloc(sizeof(seq_t) * new_size);
        memset(curr, 0, sizeof(seq_t) * new_size);

        seq_t *prev = rows[max_row];           /* previous row */

        /* fill current row */
        for (idx_t kk = 2; kk <= 2 * new_row; ++kk) {
            seq_t term0 = 0, term1 = 0, term2 = 0;
            /* p = 0 term: T(prev, kk-2) */
            if (kk - 2 >= 2 && kk - 2 <= prev_max) {
                term0 = prev[kk - 2];
            }
            /* p = 1 term: 2*(kk-1)*T(prev, kk-1) */
            if (kk - 1 >= 2 && kk - 1 <= prev_max) {
                term1 = prev[kk - 1] * (seq_t)(2 * (kk - 1));
            }
            /* p = 2 term: kk*(kk-1)*T(prev, kk) */
            if (kk <= prev_max) {
                term2 = prev[kk] * (seq_t)(kk * (kk - 1));
            }
            curr[kk] = term0 + term1 + term2;
        }

        /* expand rows array if necessary */
        if (new_row >= (idx_t) (1 + ((idx_t)1))) { /* always true but keep style */
            rows = (seq_t **)realloc(rows, sizeof(seq_t *) * (new_row + 1));
        }
        rows[new_row] = curr;
        max_row = new_row;
    }

    /* Return the requested entry */
    return rows[r][k];
}
