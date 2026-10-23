/*
 * A126953  Triangle T(n,k), 0 <= k <= n, read by rows given by: T(0,0)=1,
 *          T(n,k)=0 if k < 0 or if k > n, T(n,0) = 3*T(n-1,0) + T(n-1,1),
 *          T(n,k) = T(n-1,k-1) + T(n-1,k+1) for k >= 1.
 * https://oeis.org/A126953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 54, u32 n <= 189, u64 n <= 702,
 *            u128 n <= 1274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A126953: triangle T(n,k) read by rows (OEIS). */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* static state: cached triangle rows */
static seq_t **rows = NULL;          /* array of pointers to each row */
static idx_t rows_computed = 0;      /* number of rows actually computed */
static idx_t rows_allocated = 0;     /* allocated capacity for rows */

/* Ensure rows 0..r are computed and stored in 'rows'. */
static void ensure_rows(idx_t r)
{
    /* Grow the array of row pointers if needed */
    if (rows_allocated <= r) {
        idx_t new_alloc = rows_allocated ? rows_allocated * 2 : 1;
        while (new_alloc <= r)
            new_alloc *= 2;
        seq_t **tmp = (seq_t**)realloc(rows, new_alloc * sizeof(seq_t*));
        if (rows == NULL) {
            memset(tmp, 0, new_alloc * sizeof(seq_t*));
        } else {
            memset(tmp + rows_allocated, 0,
                   (new_alloc - rows_allocated) * sizeof(seq_t*));
        }
        rows = tmp;
        rows_allocated = new_alloc;
    }

    /* Compute missing rows */
    for (idx_t i = rows_computed; i <= r; ++i) {
        size_t len = (size_t)i + 1;
        seq_t *row = (seq_t*)malloc(len * sizeof(seq_t));
        if (row == NULL) abort();

        if (i == 0) {
            row[0] = 1;                /* T(0,0)=1 */
        } else {
            seq_t *prev = rows[i-1];
            seq_t zero = 0;

            /* first element */
            seq_t val0;
            if (i == 1) {
                /* T(1,0) = 3*T(0,0) + 0 */
                val0 = 3 * prev[0];
            } else {
                /* T(i,0) = 3*T(i-1,0) + T(i-1,1) */
                val0 = 3 * prev[0] + prev[1];
            }
            row[0] = val0;

            /* remaining elements */
            for (idx_t k = 1; k <= i; ++k) {
                seq_t left  = (k-1 <= i-1) ? prev[k-1] : zero;
                seq_t right = (k+1 <= i-1) ? prev[k+1] : zero;
                row[k] = left + right;   /* T(i,k) = T(i-1,k-1) + T(i-1,k+1) */
            }
        }

        rows[i] = row;
    }
    rows_computed = r + 1;
}

/* Return the nth term of sequence A126953 (row‑wise triangle read). */
seq_t a(idx_t n)
{
    /* Locate the row containing the index n */
    idx_t r = 0;
    while (1) {
        __int128 prod = (__int128)(r + 1) * (r + 2) / 2;  /* (r+1)(r+2)/2 */
        if (prod > n) break;
        ++r;
    }

    /* Position within that row */
    idx_t start = (r * (r + 1)) / 2;
    idx_t k = n - start;

    /* Ensure triangle up to row r */
    ensure_rows(r);

    return rows[r][k];
}
