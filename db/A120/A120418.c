/*
 * A120418  Triangle read by rows, where t(n,1) = 1, t(n,m) = t(n,m-1) +
 *          (largest square in row {n-1}).
 * https://oeis.org/A120418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 45, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A120418: Triangle read by rows.
 * Each row n (n≥1) has n terms defined by
 *   t(n,1) = 1
 *   t(n,m) = t(n,m‑1) + S(n‑1)
 * where S(n) is the largest square appearing in row n.
 * This implementation generates rows on demand and returns the
 * n-th term of the triangle.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == (idx_t)1)
        return (seq_t)1;   /* first term */

    /* row 1 has been processed: its largest square is 1 */
    seq_t step = (seq_t)1;      /* step to generate the next row   */
    idx_t total_terms = (idx_t)1; /* terms produced so far (row 1) */
    idx_t row       = (idx_t)2;   /* next row index to generate     */

    while (true)
    {
        idx_t len = row;               /* length of current row   */
        if (total_terms + len >= n)    /* the requested term is in this row */
        {
            idx_t pos_in_row = n - total_terms;                 /* 1‑based */
            return (seq_t)1 + (seq_t)(pos_in_row - 1) * step;
        }

        /* Compute largest square in this row to serve as the step for the next row. */
        seq_t largest_square = (seq_t)0;
        for (idx_t k = len; k-- > 0; )
        {
            seq_t val = (seq_t)1 + (seq_t)k * step;
            seq_t root = floor_sqrt(val);
            if (root * root == val)
            {
                largest_square = val;
                break;
            }
        }
        /* In the worst case (no other square), val=1 when k=0 will be hit, so
           largest_square is always at least 1. */

        step = largest_square;          /* step for the next row        */
        total_terms += len;             /* update cumulative term count */
        ++row;                          /* advance to next row          */
    }
}
