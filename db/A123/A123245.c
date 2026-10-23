/*
 * A123245  Triangle A079487 with reversed rows.
 * https://oeis.org/A123245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = A123245 – triangle A079487 read by rows. Triangle
 *            definition: T(0,0) = 1 T(1,0) = T(1,1) = 1 k even : T(k,j) =
 *            T(k-1,j-1) + T(k-2,j) k odd : T(k,j) = T(k-1,j) + T(k-2,j-2)
 *            a(n) is the entry at row r, column c where the concatenation of
 *            the rows 0,1,2,… produces the sequence
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers – no stdio, only what is required for memory. */
#include <stddef.h>
#include <stdlib.h>

/* --------------------------------------------------------------------- */
/*  Storage of all generated rows – rows[0] … rows[r] each an array   */
/*  of (r+1) seq_t values.  The table grows only when new rows are     */
/*  required.  The rows are kept forever to allow random order calls.   */
/* --------------------------------------------------------------------- */
static seq_t **rows = NULL;   /* array of pointers to each full row   */
static size_t row_count = 0;  /* how many rows are currently stored   */
static size_t row_cap   = 0;  /* capacity of the rows array           */

/* --------------------------------------------------------------------- */
/*  Ensure that rows[0 .. r_target] exist.                              */
/* --------------------------------------------------------------------- */
static void ensure_rows(size_t r_target)
{
    /* Nothing to do if all required rows are already present.           */
    if (r_target < row_count)
        return;

    /* Grow the pointer array if necessary.                             */
    if (row_cap < r_target + 1) {
        size_t new_cap = row_cap ? row_cap * 2 : 8;
        if (new_cap < r_target + 1)
            new_cap = r_target + 1;
        seq_t **new_rows = (seq_t **)malloc(new_cap * sizeof(seq_t *));
        if (!new_rows)          /* OOM – abort so we don't continue corruptly */
            abort();
        /* copy old pointers */
        for (size_t i = 0; i < row_cap; ++i)
            new_rows[i] = rows[i];
        /* initialise new slots with NULL */
        for (size_t i = row_cap; i < new_cap; ++i)
            new_rows[i] = NULL;
        free(rows);
        rows = new_rows;
        row_cap = new_cap;
    }

    /* Generate missing rows one by one.                                 */
    for (size_t i = row_count; i <= r_target; ++i) {
        if (i == 0) {                          /* first row */
            rows[0] = (seq_t *)malloc(sizeof(seq_t));
            rows[0][0] = (seq_t)1;
            continue;
        }
        if (i == 1) {                          /* second row */
            rows[1] = (seq_t *)malloc(2 * sizeof(seq_t));
            rows[1][0] = (seq_t)1;
            rows[1][1] = (seq_t)1;
            continue;
        }

        /* General case: allocate row of length i+1.                     */
        size_t len = i + 1;
        seq_t *ri = (seq_t *)malloc(len * sizeof(seq_t));
        if (!ri)
            abort();

        /* Fill row i according to the recurrence.                      */
        for (size_t j = 0; j < len; ++j) {
            seq_t val = (seq_t)0;
            if ((i & 1) == 0) {            /* even row  k = i           */
                /* T(i,j) = T(i-1,j-1) + T(i-2,j)                         */
                if (j > 0)                                 /* j-1 valid                     */
                    val += rows[i-1][j-1];
                /* T(i-2,j) exists only when j <= i-2 (row i-2 length i-1) */
                if (j <= i-2)
                    val += rows[i-2][j];
            } else {                         /* odd row   k = i           */
                /* T(i,j) = T(i-1,j) + T(i-2,j-2)                          */
                if (j <= i-1)                                 /* j within row i-1 length i   */
                    val += rows[i-1][j];
                if (j >= 2 && (j-2) <= i-2)                   /* T(i-2,j-2) exists          */
                    val += rows[i-2][j-2];
            }
            ri[j] = val;
        }
        rows[i] = ri;
    }
    row_count = r_target + 1;
}

/* --------------------------------------------------------------------- */
/*  Public sequence function                                             */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Locate the row that contains the n‑th element of the flattened   */
    /* triangle.  The rows have lengths 1,2,3,… so cumulative count is   */
    /* triangular numbers.  We find minimal r with 1+2+…+(r+1)>n.          */
    size_t r = 0;
    idx_t total = 0;                /* number of elements before row r  */

    while (total + (idx_t)(r + 1) <= n) {
        total += (idx_t)(r + 1);
        ++r;
    }
    size_t col = (size_t)(n - total);   /* column index within row r     */

    ensure_rows(r);                 /* make sure rows[0..r] exist     */
    return rows[r][col];
}
