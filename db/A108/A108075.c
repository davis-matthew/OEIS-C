/*
 * A108075  Triangle in A071945 with rows reversed.
 * https://oeis.org/A108075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 54, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A108075 – Flattened, row‑reversed triangle defined by:
        T(n,k) = T(n-1,k-1) + T(n-2,k-1) + T(n,k+1)   (T(0,0)=1)
    The sequence is obtained by listing each row of the triangle
    from right to left (i.e. reversed) and concatenating the rows.  */

#include <stdlib.h>

static seq_t **rows = NULL;   /*  Pointer to array of row pointers          */
static idx_t  max_row = 0;    /*  Highest row that has been computed        */

/*---------------------------------------------------------------------*/
/*  Ensure that rows 0 … r are computed                                */
/*---------------------------------------------------------------------*/
static void ensure_rows(idx_t r)
{
    /* First invoke: create row 0 */
    if (rows == NULL) {
        rows = (seq_t **)malloc(sizeof(seq_t *));
        if (!rows) exit(EXIT_FAILURE);
        seq_t *row0 = (seq_t *)malloc(sizeof(seq_t));
        if (!row0) exit(EXIT_FAILURE);
        row0[0] = (seq_t)1;          /*  T(0,0) = 1  */
        rows[0] = row0;
        max_row = 0;
    }

    if (r <= max_row) return;        /* already available */

    /* Extend the array of row pointers */
    seq_t **new_rows = (seq_t **)realloc(rows, (r + 1) * sizeof(seq_t *));
    if (!new_rows) exit(EXIT_FAILURE);
    rows = new_rows;

    /* Compute the new rows */
    for (idx_t i = max_row + 1; i <= r; ++i) {
        idx_t len = i + 1;            /* number of entries in row i */
        seq_t *row = (seq_t *)malloc(len * sizeof(seq_t));
        if (!row) exit(EXIT_FAILURE);

        if (i == 0) {                 /* already handled in the first call */
            row[0] = (seq_t)1;
        } else {
            /* Build row i from right to left because T(i,k) depends on T(i,k+1) */
            for (idx_t k = i; ; --k) {
                seq_t val = (seq_t)0;

                /* T(i-1,k-1) */
                if (k > 0)
                    val += rows[i - 1][k - 1];

                /* T(i-2,k-1) – only if the previous row exists and k-1 is valid */
                if (i >= 2 && k > 0 && k < i)
                    val += rows[i - 2][k - 1];

                /* T(i,k+1) – already computed in this row */
                if (k + 1 <= i)
                    val += row[k + 1];

                row[k] = val;

                if (k == 0) break;        /* finished current row */
            }
        }
        rows[i] = row;
    }

    max_row = r;
}

/*---------------------------------------------------------------------*/
/*  Return the n‑th term of sequence A108075.                          */
/*---------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Determine the row that contains index n */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    /* Position within that row (0‑based, leftmost after reversal) */
    idx_t row_start = r * (r + 1) / 2;
    idx_t pos       = n - row_start;

    /* Ensure rows up to r are computed */
    ensure_rows(r);

    return rows[r][pos];
}
