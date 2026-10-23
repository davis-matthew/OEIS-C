/*
 * A100578  Table read by rows: T(n,k) = if n<=k then n else
 *          Sum(T(n-i,k):1<=i<=k), 1<=k<=n, triangle read by rows.
 * https://oeis.org/A100578
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: T(n,k) = if n<=k then n
 *                 else sum_{i=1}^{k} T(n-i,k)
 * The triangle is read by rows and flattened into a single sequence a(n). */
#include <stdlib.h>
#include <stdint.h>

static seq_t **rows = NULL;          /* pointers to rows, 1‑based rows -> 0‑based array */
static idx_t  max_row          = 0;  /* highest row computed so far (1‑based) */
static idx_t  cumulative_total = 0;  /* total number of sequence terms computed so far */

/* Return the nth term of A100578.  n is 1‑based. */
seq_t a(idx_t n)
{
    /* Generate rows until we have at least n terms. */
    while (cumulative_total < n) {
        /* Add a new row */
        max_row += 1;                                   /* new row number (1‑based) */

        rows = (seq_t **)realloc(rows, max_row * sizeof(seq_t *));
        if (!rows) return (seq_t)0;   /* out of memory – unlikely in tests */

        /* Allocate space for the new row (index 0 unused). */
        rows[max_row - 1] = (seq_t *)calloc((size_t)(max_row + 1), sizeof(seq_t));
        if (!rows[max_row - 1]) return (seq_t)0; /* out of memory */

        /* Compute the entries of the new row. */
        for (idx_t k = 1; k <= max_row; ++k) {
            if (k == max_row) {                  /* T(n,n) = n */
                rows[max_row - 1][k] = (seq_t)max_row;
            } else {
                seq_t sum = 0;
                for (idx_t i = 1; i <= k; ++i) {
                    idx_t row_num = max_row - i;  /* previous row number */
                    if (row_num <= k) {           /* n <= k: value is the row number itself */
                        sum += (seq_t)row_num;
                    } else {
                        sum += rows[row_num - 1][k]; /* use stored value */
                    }
                }
                rows[max_row - 1][k] = sum;
            }
        }

        cumulative_total += max_row;                     /* update total terms count */
    }

    /* The desired term lies in the current max_row. */
    idx_t pos = n - (cumulative_total - max_row);      /* 1‑based column within the row */
    return rows[max_row - 1][pos];
}
