/*
 * A203412  Triangle read by rows, a(n,k), n>=k>=1, which represent the s=3,
 *          h=1 case of a two-parameter generalization of Stirling numbers
 *          arising in conjunction with normal ordering.
 * https://oeis.org/A203412
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = triangle read by rows of a(n,k) with a(n+1,k) =
 *            a(n,k-1)+(3*n-2*k)*a(n,k), a(n,0) = δ_{n,0}, a(0,k) = δ_{0,k}
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes */
#include <stdlib.h>
#include <stdint.h>

/* Global cache of computed rows */
static seq_t **rows = NULL;       /* rows[1] .. rows[rows_len] */
static size_t rows_cap = 0;       /* capacity of rows array */
static idx_t rows_len = 0;        /* number of rows currently filled */

/* Ensure that rows 1..r are computed, allocating as necessary. */
static void ensure_rows(idx_t r)
{
    /* Allocate or grow the rows pointer array. */
    if (rows_len < r)
    {
        if (rows_cap < (size_t)(r + 1))
        {
            size_t new_cap = (rows_cap == 0) ? (size_t)(r + 1) : rows_cap * 2;
            if (new_cap < (size_t)(r + 1))
                new_cap = (size_t)(r + 1);
            seq_t **new_rows = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
            if (!new_rows) exit(1);
            rows = new_rows;
            rows_cap = new_cap;
        }

        /* Initialise row 1 if needed. */
        if (rows_len == 0)
        {
            seq_t *row1 = (seq_t*)malloc(2 * sizeof(seq_t));  /* indices 0..1 */
            if (!row1) exit(1);
            row1[0] = 0;          /* a(1,0) = 0 (unused) */
            row1[1] = 1;          /* a(1,1) = 1 */
            rows[1] = row1;
            rows_len = 1;
        }

        /* Compute rows from rows_len+1 up to r. */
        for (idx_t cur = rows_len + 1; cur <= r; ++cur)
        {
            seq_t *prev = rows[cur - 1];
            seq_t *cur_row = (seq_t*)malloc(((size_t)cur + 1) * sizeof(seq_t));
            if (!cur_row) exit(1);
            cur_row[0] = 0;  /* a(cur,0) = 0 (unused) */

            for (idx_t k = 1; k <= cur; ++k)
            {
                seq_t val = 0;
                if (k > 1)  /* a(cur-1,k-1) */
                    val += prev[k - 1];

                if (k <= cur - 1)  /* a(cur-1,k) contributes */
                {
                    seq_t coeff = (seq_t)( (seq_t)(3 * (cur - 1)) - (seq_t)(2 * k) );
                    val += coeff * prev[k];
                }
                cur_row[k] = val;
            }
            rows[cur] = cur_row;
        }
        rows_len = r;
    }
}

/* Return the OEIS sequence A203412 term at position n (n>=1). */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n<1 */

    /* Find row r such that r*(r+1)/2 >= n. */
    idx_t r = 1;
    while ((idx_t)r * (r + 1) / 2 < n) ++r;

    /* Column index k (1‑based). */
    idx_t k = n - (idx_t)r * (r - 1) / 2;

    ensure_rows(r);
    return rows[r][k];
}
