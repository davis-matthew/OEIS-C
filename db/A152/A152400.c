/*
 * A152400  Triangle T, read by rows, where column k of T = column 0 of matrix
 *          power T^(k+1) for k>0, with column 0 of T = unsigned column 0 of
 *          T^-1 (shifted).
 * https://oeis.org/A152400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 35, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A152400 – Triangle T read by rows.  
   T(0,0)=1.  For k>0:  T(n,k)=Σ_{j=0..n-k}T(n‑k,j)*T(j+k‑1,k‑1) (n≥k>0).  
   For k=0:  T(n,0)=Σ_{j=1..n}T(n,j)*T(j‑1,0) (n≥0).  
   The sequence a(n) consists of the entries of T read row by row:  
   a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), a(4)=T(2,1), a(5)=T(2,2), …
*/

#include <stdlib.h>
#include <stdint.h>

/* global storage for the triangular rows and the flattened sequence */
static seq_t **row_ptr  = NULL;   /* pointers to each row */
static size_t *row_len   = NULL;   /* lengths of each row */
static size_t rows_alloc = 0;      /* allocated number of rows */
static size_t rows_used  = 0;      /* number of rows actually stored */

static seq_t *a_vals   = NULL;   /* flattened triangle */
static size_t a_alloc  = 0;       /* allocated size of a_vals */
static size_t a_used   = 0;       /* number of entries stored */

/* Ensure storage for at least 'n' rows */
static void ensure_rows(size_t n)
{
    if (n <= rows_alloc)
        return;
    size_t new_cap = rows_alloc ? rows_alloc * 2 : 1;
    while (new_cap < n)
        new_cap <<= 1;
    seq_t **tmp = (seq_t**)realloc(row_ptr, new_cap * sizeof(seq_t*));
    if (!tmp) exit(EXIT_FAILURE);
    row_ptr = tmp;
    size_t *tmp2 = (size_t*)realloc(row_len, new_cap * sizeof(size_t));
    if (!tmp2) exit(EXIT_FAILURE);
    row_len = tmp2;
    rows_alloc = new_cap;
}

/* Ensure storage for at least 'n' flattened values */
static void ensure_a(size_t n)
{
    if (n <= a_alloc)
        return;
    size_t new_cap = a_alloc ? a_alloc * 2 : 1;
    while (new_cap < n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(a_vals, new_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    a_vals = tmp;
    a_alloc = new_cap;
}

/* Compute all rows and flattened entries up to the given index 'n' */
static void compute_up_to(idx_t n)
{
    if (a_used > 0 && (size_t)n < a_used)
        return;                         /* already computed up to earlier index */

    /* Initialize first row if not yet present */
    if (rows_used == 0) {
        ensure_rows(1);
        row_ptr[0] = (seq_t*)calloc(1, sizeof(seq_t));
        if (!row_ptr[0]) exit(EXIT_FAILURE);
        row_len[0] = 1;
        row_ptr[0][0] = (seq_t)1;
        rows_used = 1;

        ensure_a(1);
        a_vals[0] = (seq_t)1;
        a_used = 1;
    }

    /* Generate subsequent rows until we have at least n+1 values */
    while (a_used <= (size_t)n) {
        size_t r = rows_used;               /* new row index, 0‑based */
        ensure_rows(rows_used + 1);
        size_t len = r + 1;
        row_ptr[r] = (seq_t*)calloc(len, sizeof(seq_t));
        if (!row_ptr[r]) exit(EXIT_FAILURE);
        row_len[r] = len;

        seq_t *row = row_ptr[r];

        /* Compute columns k > 0 using the recurrence */
        for (size_t k = 1; k <= r; ++k) {
            seq_t sum = 0;
            size_t max_j = r - k;
            for (size_t j = 0; j <= max_j; ++j) {
                seq_t a = row_ptr[r - k][j];          /* T(r‑k, j)  */
                seq_t b = row_ptr[j + k - 1][k - 1];  /* T(j+k‑1, k‑1) */
                seq_t prod = a * b;                    /* exact for seq_t */
                sum += prod;
            }
            row[k] = sum;
        }

        /* Compute column 0 using the second recurrence */
        seq_t sum0 = 0;
        for (size_t j = 1; j <= r; ++j) {
            seq_t a = row[j];
            seq_t b = row_ptr[j - 1][0];
            seq_t prod = a * b;
            sum0 += prod;
        }
        row[0] = sum0;

        /* Append the new row to the flattened list */
        ensure_a(a_used + len);
        for (size_t c = 0; c < len; ++c)
            a_vals[a_used++] = row[c];

        rows_used++;
    }
}

seq_t a(idx_t n)
{
    compute_up_to(n);
    return a_vals[(size_t)n];
}
