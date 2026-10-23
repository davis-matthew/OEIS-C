/*
 * A135299  Pascal's triangle, but the last element of the row is the sum of
 *          all the previous terms.
 * https://oeis.org/A135299
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 53, u32 n <= 169, u64 n <= 324,
 *            u128 n <= 324, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A135299 – row‑major enumeration of the triangle defined in OEIS A135299.
 *
 *  Formula: a(n) is the element T(r,k) of that triangle, where
 *          T(0,0) = 1;
 *          T(n,0) = 1;
 *          T(n,k) = T(n-1,k-1) + T(n-1,k)          for 1 ≤ k < n;
 *          T(n,n) = (sum of all earlier triangle entries)
 *                   + (sum of the earlier entries in row n).
 *
 *  The function returns a(n) for any n ≥ 0, using only O(r²) arithmetic
 *  where r ≈ √(2n).  The implementation keeps two rows of the triangle
 *  in memory and is safe for all values that fit in seq_t.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row r and column k of the nth entry (row‑major order). */
    idx_t r = 0;
    idx_t cum = 0;                      /* cumulative number of entries up to row r */
    while (1) {
        cum += r + 1;                   /* add current row length (r+1) */
        if (n < cum) break;
        ++r;
    }
    idx_t k = n - (cum - (r + 1));      /* position inside the row (0‑based) */

    /* Allocate two buffers of length r+1 to hold previous and current rows. */
    size_t len = (size_t)(r + 1);
    seq_t *prev = (seq_t*)malloc(len * sizeof(seq_t));
    seq_t *cur  = (seq_t*)malloc(len * sizeof(seq_t));
    if (!prev || !cur) {
        free(prev);
        free(cur);
        return (seq_t)0;                /* allocation failure: return 0 */
    }

    seq_t sum_before = 0;               /* sum of all entries in rows < current row */

    for (idx_t i = 0; i <= r; ++i) {
        /* First element of every row is 1. */
        cur[0] = 1;
        seq_t sum_row_without_last = 1; /* running sum of entries in this row,
                                          excluding the last one */

        if (i > 0) {
            /* Interior elements (k = 1 … i−1). */
            for (idx_t j = 1; j < i; ++j) {
                cur[j] = prev[j - 1] + prev[j];
                sum_row_without_last += cur[j];
            }
            /* Last element: sum of all previous entries plus sum of earlier
               entries in this row. */
            seq_t last = sum_before + sum_row_without_last;
            cur[i] = last;
            sum_row_without_last += last; /* now sum of the complete row */
        }
        /* Update the cumulative sum for the next row. */
        sum_before += sum_row_without_last;

        /* Swap prev and cur for the next iteration. */
        seq_t *tmp = prev;
        prev = cur;
        cur  = tmp;
    }

    seq_t result = prev[(size_t)k];     /* 'prev' now holds row r */

    free(prev);
    free(cur);
    return result;
}
