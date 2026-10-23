/*
 * A185410  A decomposition of the double factorials A001147.
 * https://oeis.org/A185410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is the (n‑th) entry of the triangle T(n,k) defined by
 *            T(n,0)=1, and for n≥1, k≥1 T(n,k) = (2k+1)·T(n‑1,k) +
 *            2(n−k)·T(n‑1,k‑1). The sequence a(n) is obtained by listing the
 *            entries row by row (row 0 has length 1, row 1 length 2, …)
 * Range:     u8 n <= 16, u16 n <= 37, u32 n <= 70, u64 n <= 177,
 *            u128 n <= 475, big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

static seq_t **triangle = NULL;          /* rows of the triangle        */
static idx_t *row_start = NULL;          /* index where each row starts */
static idx_t row_cap = 0;                /* allocated capacity for rows */
static idx_t row_count = 0;              /* number of rows computed     */
static idx_t cum_count = 0;              /* total elements in rows      */

/* grow storage to at least new_cap rows                                            */
static void grow_storage(idx_t new_cap) {
    triangle = (seq_t **)realloc(triangle, new_cap * sizeof(seq_t *));
    row_start = (idx_t *)realloc(row_start, new_cap * sizeof(idx_t));
    row_cap = new_cap;
}

/* ensure rows up to the one containing index n are computed                       */
static void ensure_rows(idx_t n) {
    while (cum_count <= n) {
        idx_t r = row_count;                      /* new row index   */
        /* grow storage if needed */
        if (row_count >= row_cap)
            grow_storage(row_cap ? row_cap * 2 : 16);

        /* allocate row r of length r+1 */
        seq_t *row = (seq_t *)malloc((r + 1) * sizeof(seq_t));
        if (!row) abort();

        /* store start index of this row */
        row_start[r] = cum_count;

        /* first element always 1 */
        row[0] = 1;

        /* compute remaining elements using recurrence */
        if (r > 0) {
            seq_t *prev = triangle[r - 1];
            for (idx_t k = 1; k <= r; ++k) {
                seq_t a = 0;                     /* term1 */
                if (k <= r - 1)
                    a = mul_small(prev[k], (unsigned int)(2 * k + 1));
                seq_t b = mul_small(prev[k - 1], (unsigned int)(2 * (r - k)));
                row[k] = a + b;
            }
        }

        triangle[r] = row;
        cum_count += r + 1;
        row_count++;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 1;         /* first element explicitly known */
    ensure_rows(n);

    /* locate the row containing n */
    idx_t r;
    for (r = 0; r < row_count; ++r) {
        if (n < row_start[r] + (r + 1))
            break;
    }
    idx_t offset = n - row_start[r];
    return triangle[r][offset];
}
