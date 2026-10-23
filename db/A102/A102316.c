/*
 * A102316  Triangle, read by rows, where T(n,k) = T(n,k-1) + (k+1)*T(n-1,k)
 *          for n>k>0, T(n,0)=1 and T(n,n) = T(n,n-1) for n>=0.
 * https://oeis.org/A102316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, c) where T is defined by * T(n,0) = 1 * T(n,n) =
 *            T(n,n-1) (for n ≥ 0) * T(n,k) = T(n,k-1) + (k+1)·T(n-1,k) (for
 *            n>k>0) * The sequence a(n) is the triangle read by rows
 * Range:     u8 n <= 17, u16 n <= 31, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n)
        ++r;

    /* Column index within the row */
    idx_t col = n - r * (r + 1) / 2;

    /* Static state for storing computed rows */
    static seq_t **rows = NULL;          /* array of pointers to rows */
    static idx_t  rows_capacity = 0;     /* currently allocated slots */
    static idx_t  rows_count = 0;        /* number of rows computed */

    /* Ensure the rows array has room for row r */
    if (r >= rows_capacity) {
        idx_t new_cap = rows_capacity ? rows_capacity * 2 : 4;
        if (new_cap <= r) new_cap = r + 4;
        seq_t **tmp = (seq_t**) realloc(rows, new_cap * sizeof(seq_t *));
        rows = tmp;
        for (idx_t i = rows_capacity; i < new_cap; ++i)
            rows[i] = NULL;
        rows_capacity = new_cap;
    }

    /* Compute rows up to r if not already */
    while (rows_count <= r) {
        idx_t cur = rows_count;   /* compute row cur */
        seq_t *row;
#ifdef __cplusplus
        row = new seq_t[cur + 1];
#else
        row = (seq_t*) malloc((cur + 1) * sizeof(seq_t));
#endif
        row[0] = 1;

        if (cur > 0) {
            seq_t *prev = rows[cur - 1];
            for (idx_t k = 1; k < cur; ++k) {
                /* T(cur,k) = T(cur,k-1) + (k+1)·T(cur-1,k) */
                row[k] = row[k - 1] + ((seq_t)(k + 1)) * prev[k];
            }
            /* The diagonal element T(cur,cur) = T(cur,cur-1) */
            row[cur] = row[cur - 1];
        }
        rows[cur] = row;
        ++rows_count;
    }

    return rows[r][col];
}
