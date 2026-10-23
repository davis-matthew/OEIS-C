/*
 * A167666  Triangle read by rows given by [1,1,-4,2,0,0,0,0,0,0,0,...] DELTA
 *          [1,0,0,1,0,0,0,0,0,0,0,...] where DELTA is the operator defined in
 *          A084938.
 * https://oeis.org/A167666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle T(n,k) defined by
 *   T(0,0)=1,
 *   T(1,0)=T(1,1)=1,
 *   T(2,0)=2, T(2,1)=3, T(2,2)=1,
 *   T(n,k) = 2*T(n-1,k-1) - T(n-2,k-2)   for k>=2.
 * a(n) is the triangle read by rows:
 *   a(n) = T(row,col)  where row is the largest r with r*(r+1)/2 <= n
 *   and col = n - r*(r+1)/2.
 */
#include <stdlib.h>

/* static cache of computed rows */
static seq_t **rows = NULL;    /* pointer to array of row pointers   */
static size_t rows_count = 0;  /* number of rows currently computed */
static size_t rows_cap   = 0;  /* allocated capacity of rows array    */

/* ensure rows array has at least 'needed' slots */
static void ensure_rows_capacity(size_t needed)
{
    if (needed <= rows_cap)
        return;
    size_t new_cap = rows_cap ? rows_cap * 2 : 8;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t **tmp = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
    rows = tmp;
    rows_cap = new_cap;
}

/* add a single row with index idx (0‑based) using the recurrence */
static void add_row(size_t idx)
{
    size_t len = idx + 1;
    seq_t *row = (seq_t*)malloc(len * sizeof(seq_t));
    if (!row) return;  /* allocation failure – silently ignore */

    if (idx == 0) {
        row[0] = (seq_t)1;
    } else if (idx == 1) {
        row[0] = (seq_t)1;
        row[1] = (seq_t)1;
    } else if (idx == 2) {
        row[0] = (seq_t)2;
        row[1] = (seq_t)3;
        row[2] = (seq_t)1;
    } else {
        /* general case */
        row[0] = (seq_t)0;
        if (idx == 3)
            row[1] = (seq_t)4;      /* T(3,1) = 4 */
        else
            row[1] = (seq_t)0;      /* T(n,1)=0 for n>=4 */

        /* compute for k >= 2 using recurrence */
        for (size_t k = 2; k <= idx; ++k) {
            seq_t a = rows[idx-1][k-1]; /* T(n-1,k-1) */
            seq_t b = rows[idx-2][k-2]; /* T(n-2,k-2) */
            row[k] = (seq_t)((seq_t)2 * a - b);
        }
    }

    rows[idx] = row;
}

/* public function required by the task */
seq_t a(idx_t n)
{
    /* determine the row r that contains the nth element */
    size_t r = 0;
    while (((r+1) * (r+2)) / 2 <= (unsigned long long)n)
        ++r;

    /* generate rows up to r if necessary */
    if (rows_count <= r) {
        ensure_rows_capacity(r + 1);
        while (rows_count <= r) {
            add_row(rows_count);
            ++rows_count;
        }
    }

    size_t row_start = r * (r + 1) / 2;
    size_t k = (size_t)(n - row_start);
    return rows[r][k];
}
