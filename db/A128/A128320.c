/*
 * A128320  Triangle, read by rows, where T(n,k) equals the dot product of the
 *          vector of terms in row n that are to the right of T(n,k) with the
 *          vector of terms in column k that are above T(n,k) for n>k+1>0,
 *          with the odd numbers in the secondary diagonal and all 1's in the
 *          main diagonal.
 * https://oeis.org/A128320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the value T(r,c) of the triangle defined by T(n,n)=1,
 *            T(n,n-1)=2*n-1; for n>k+1≥1, T(n,k)=∑_{j=0..n-1-k} T(n,k+j+1) *
 *            T(k+j,k). Initial rows: T(0,0)=1, T(1,0)=1, T(1,1)=1
 * Range:     u8 n <= 14, u16 n <= 35, u32 n <= 90, u64 n <= 230,
 *            u128 n <= 702, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *tri_data = NULL;   /* contiguous storage of all rows   */
static idx_t  data_cap = 0;      /* allocated capacity of tri_data   */
static idx_t  data_len = 0;      /* number of elements written       */

static idx_t *row_offs = NULL;   /* start index of each row in tri_data */
static idx_t  row_cap  = 0;      /* allocated capacity of row_offs   */
static idx_t  row_count = 0;     /* number of rows already computed  */

/* Ensure that rows 0 .. r are computed. */
static void ensure_rows(idx_t r)
{
    if (row_count > r) return;

    /* Allocate or grow the row_offs array as needed. */
    if (row_count + 1 > row_cap) {
        idx_t new_cap = row_cap ? row_cap * 2 : 8;
        if (new_cap < row_count + 1) new_cap = row_count + 1;
        idx_t *tmp = (idx_t *)realloc(row_offs, new_cap * sizeof(idx_t));
        if (!tmp) abort();
        row_offs = tmp;
        row_cap  = new_cap;
    }

    for (idx_t rr = row_count; rr <= r; ++rr) {
        idx_t len = rr + 1;           /* number of elements in this row */

        /* Allocate or grow the data buffer. */
        if (data_len + len > data_cap) {
            idx_t new_cap = data_cap ? data_cap * 2 : 128;
            if (new_cap < data_len + len) new_cap = data_len + len;
            seq_t *tmp = (seq_t *)realloc(tri_data, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            tri_data = tmp;
            data_cap = new_cap;
        }

        seq_t *start = tri_data + data_len;
        row_offs[rr] = data_len;        /* record start offset of this row */
        data_len += len;                /* update total length */

        /* Compute the elements of this row. */
        if (rr == 0) {
            start[0] = (seq_t)1;
        } else if (rr == 1) {
            start[0] = (seq_t)(2 * 1 - 1);  /* secondary diagonal */
            start[1] = (seq_t)1;
        } else {
            /* main diagonal */
            start[rr] = (seq_t)1;
            /* secondary diagonal */
            start[rr - 1] = (seq_t)(2 * rr - 1);

            /* Remaining entries, computed from the recurrence. */
            for (idx_t k = rr - 2; ; --k) {
                seq_t sum = 0;
                idx_t limit = rr - 1 - k;
                for (idx_t j = 0; j <= limit; ++j) {
                    seq_t val1 = start[k + j + 1];
                    seq_t val2 =
                        tri_data[row_offs[k + j] + k];
                    sum += val1 * val2;
                }
                start[k] = sum;
                if (k == 0) break;
            }
        }
    }

    row_count = r + 1;
}

/* Return the value a(n). */
seq_t a(idx_t n)
{
    /* Find the row that contains index n. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;

    /* Ensure rows up to r exist. */
    ensure_rows(r);

    idx_t offset = r * (r + 1) / 2;
    idx_t c = n - offset;
    return tri_data[row_offs[r] + c];
}
