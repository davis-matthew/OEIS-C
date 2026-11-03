/*
 * A064881  Eisenstein array Ei(1,2).
 * https://oeis.org/A064881
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 69 terms through n = 69 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A064881 – Eisenstein array Ei(1,2)
   a(1,0)=1, a(1,1)=2
   For n>1:
     if m even: a(n,m)=a(n-1, m/2)
     if m odd : a(n,m)=a(n-1, (m-1)/2)+a(n-1, (m+1)/2)
   The flattened index n (starting at 1) runs row by row
   (rows vary in length 2^(row-1)+1).  This function returns
   a(n) – the value at the n-th position in that flattened array. */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include "../utils/mem.h"

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

/* Compute the row number and zero‑based position m inside that row
   for a given flat index idx (1‑based).  The results are stored
   in *row and *m. */
static void row_and_offset(idx_t idx, idx_t *row, idx_t *m)
{
    idx_t cum = 0;
    idx_t r   = 0;
    while (cum < idx) {
        r++;
        /* length of row r is 2^(r-1)+1 */
        cum += ((idx_t)1 << (r-1)) + 1;
    }
    idx_t len   = ((idx_t)1 << (r-1)) + 1;
    idx_t before = cum - len;          /* number of elements before this row */
    *row = r;
    *m   = idx - before - 1;           /* zero‑based offset within the row */
}

/* Cache for all complete rows that have been built so far.
   row_cache[1] … row_cache[cache_rows] contain pointers to the
   corresponding seq_t arrays.  row_cache[0] is unused. */
static seq_t **row_cache = NULL;
static idx_t  cache_rows = 0;   /* number of rows stored (rows 1..cache_rows) */

/* Build rows up to and including 'r'.
   New rows are appended to the cache and may be reused on later calls. */
static void build_up_to(idx_t r)
{
    if (r <= cache_rows) return;       /* nothing to do */

    /* Expand the cache array to accommodate rows 1..r (index 0 unused). */
    void *tmp = realloc(row_cache, (size_t)(r + 1) * sizeof(seq_t*));
    if (tmp == NULL) abort();          /* memory exhaustion is fatal */
    row_cache = (seq_t**)tmp;

    /* Build each new row. */
    for (idx_t i = cache_rows + 1; i <= r; ++i) {
        idx_t len = ((idx_t)1 << (i-1)) + 1;
        seq_t *row = seq_alloc_new_array((size_t)len);
        row_cache[i] = row;

        if (i == 1) {                 /* base row */
            row[0] = (seq_t)1;
            row[1] = (seq_t)2;
        } else {
            seq_t *prev = row_cache[i-1];
            for (idx_t j = 0; j < len; ++j) {
                if ((j & 1) == 0) {   /* even column index */
                    row[j] = prev[j >> 1];
                } else {              /* odd column index */
                    row[j] = prev[(j - 1) >> 1] + prev[(j + 1) >> 1];
                }
            }
        }
    }
    cache_rows = r;
}

/* Return the n-th element of the flattened Eisenstein array. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* index 0 is unused by OEIS */

    idx_t row, offset;
    row_and_offset(n, &row, &offset);
    build_up_to(row);
    return row_cache[row][offset];
}
