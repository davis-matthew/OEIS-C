/*
 * A154229  Triangle T(n, k) = T(n-1, k) + T(n-1, k-1) +
 *          ((n+1)*(n+2)/2)^2*T(n-2, k-1), read by rows.
 * https://oeis.org/A154229
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 39, u64 n <= 96, u128 n <= 262,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A154229.
   Triangle recurrence:
   T(n, k) = T(n-1, k) + T(n-1, k-1) + ((n+1)*(n+2)/2)^2 * T(n-2, k-1)
   with T(n,0)=T(n,n)=1.
   a(n) is the term obtained by reading the triangle row‑by‑row
   (row 0 has 1 entry, row 1 has 2 entries, …).  Indexing starts at 0.  */

#include <stdlib.h>
#include <stdbool.h>

/* Integer square root for idx_t (unsigned 64‑bit).  */
static inline idx_t idx_sqrt(idx_t x)
{
    idx_t lo = 0, hi = 0xFFFFFFFFULL;            /* sqrt(2^64-1) fits in 32 bits */
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if (mid == 0) { lo = 1; continue; }
        if (mid <= x / mid) {
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return hi;
}

/* Static storage for the triangle rows.  */
static seq_t **tri = NULL;           /* array of row pointers */
static size_t tri_rows = 0;           /* number of rows currently stored */
static size_t tri_cap  = 0;           /* allocated capacity for tri */

/* Return a(n) for the OEIS sequence.  */
seq_t a(idx_t n)
{
    /* Find the row r and column k corresponding to the linear index n.  */
    idx_t r = (idx_sqrt((idx_t)(8 * n + 1)) - 1) / 2;
    idx_t offset = ( (idx_t)r * (idx_t)(r + 1) ) / 2;
    idx_t k = n - offset;

    /* Grow the triangle up to row r (inclusive).  */
    while (tri_rows <= (size_t)r) {
        idx_t row = (idx_t)tri_rows;
        size_t len = (size_t)row + 1;
        seq_t *row_arr = (seq_t *)malloc(len * sizeof(seq_t));
        if (!row_arr) return 0;                     /* allocation failed */

        if (row == 0) {
            row_arr[0] = (seq_t)1;
        } else if (row == 1) {
            row_arr[0] = row_arr[1] = (seq_t)1;
        } else {
            /* Boundary entries */
            row_arr[0] = row_arr[len - 1] = (seq_t)1;

            /* Factor C = ((row+1)*(row+2)/2)^2.  */
            seq_t t = ((seq_t)(row + 1) * (seq_t)(row + 2)) / 2;
            seq_t C = t * t;

            seq_t *prev    = tri[(size_t)row - 1];
            seq_t *prevprev = tri[(size_t)row - 2];

            for (size_t idx = 1; idx + 1 < len; ++idx) {
                seq_t val = prev[idx];
                val += prev[idx - 1];
                val += C * prevprev[idx - 1];
                row_arr[idx] = val;
            }
        }

        /* Ensure capacity for the rows array.  */
        if (tri_rows == tri_cap) {
            size_t new_cap = (tri_cap == 0) ? 4 : tri_cap * 2;
            seq_t **tmp = (seq_t **)realloc(tri, new_cap * sizeof(seq_t *));
            if (!tmp) { free(row_arr); return 0; }
            tri = tmp;
            tri_cap = new_cap;
        }
        tri[tri_rows++] = row_arr;
    }

    return tri[(size_t)r][(size_t)k];
}
