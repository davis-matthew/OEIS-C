/*
 * A182822  Exponential Riordan array, defining orthogonal polynomials related
 *          to permutations without double falls.
 * https://oeis.org/A182822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence a(n) = T(r,k) where (r,k) are the coordinates of the
 * n-th entry (in row‑major order) of the triangle T defined by  
 *
 *   T(0,0) = 1                     (initial value)
 *   T(n,k) = T(n-1,k-1) + (k+1)·T(n-1,k) + (k+1)²·T(n-1,k+1)
 *   for n > 0,  with T(i,j)=0 if j<0 or i<j.
 *
 * The first few rows are
 *   1;
 *   1,1;
 *   2,3,1;
 *   5,12,6,1;
 *   …
 *
 * a(0)=1, a(1)=1, a(2)=1, a(3)=2, a(4)=3, a(5)=1, … as given in the OEIS entry.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Static storage for the rows of the triangle.  */
static seq_t **triangle = NULL;  /* triangle[r] points to a row array of size r+1.  */
static idx_t *row_len   = NULL;  /* length of each row (r+1).  */
static idx_t max_row    = 0;      /* highest row computed so far.  */

/* Build all rows up to and including row r.  */
static void build_up_to(idx_t r)
{
    if (triangle == NULL) {
        /* initialise row 0 */
        triangle  = (seq_t**)malloc(sizeof(seq_t*));
        row_len   = (idx_t*)malloc(sizeof(idx_t));
        if (triangle == NULL || row_len == NULL) return; /* OOM protection */

        seq_t *row0 = (seq_t*)malloc(sizeof(seq_t));
        if (row0 == NULL) return;
        row0[0] = (seq_t)1;
        triangle[0]  = row0;
        row_len[0]   = 1;
        max_row      = 0;
    }

    /* grow the triangle until max_row >= r */
    while (max_row < r) {
        idx_t old_len = row_len[max_row];          /* = max_row + 1 */
        idx_t new_len = old_len + 1;               /* = max_row + 2 */

        seq_t *old_row = triangle[max_row];
        seq_t *new_row = (seq_t*)calloc(new_len, sizeof(seq_t));
        if (new_row == NULL) return; /* OOM protection */

        for (idx_t k = 0; k < new_len; ++k) {
            seq_t val = 0;

            if (k > 0)                     /* T(n-1,k-1) */
                val += old_row[k - 1];

            if (k < old_len) {             /* (k+1)*T(n-1,k) */
                seq_t mul = (seq_t)(k + 1);
                val += mul * old_row[k];
            }

            if (k + 1 < old_len) {         /* (k+1)²*T(n-1,k+1) */
                seq_t mul = (seq_t)(k + 1);
                val += (mul * mul) * old_row[k + 1];
            }

            new_row[k] = val;
        }

        /* reallocate storage for the next row pointer and length */
        seq_t **new_tri      = (seq_t**)malloc(sizeof(seq_t*) * (max_row + 2));
        idx_t  *new_len_arr  = (idx_t*)malloc(sizeof(idx_t) * (max_row + 2));
        if (new_tri == NULL || new_len_arr == NULL) return;

        for (idx_t i = 0; i < max_row + 1; ++i) {
            new_tri[i] = triangle[i];
            new_len_arr[i] = row_len[i];
        }
        new_tri[max_row + 1] = new_row;
        new_len_arr[max_row + 1] = new_len;

        free(triangle);
        free(row_len);
        triangle = new_tri;
        row_len  = new_len_arr;
        ++max_row;
    }
}

/* Public interface: a(n) */
seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2  */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) {
        ++r;
    }
    idx_t prev_total = r * (r + 1) / 2;
    idx_t k = n - prev_total;     /* column index within row r */

    build_up_to(r);                /* ensure the needed row is available */
    return triangle[r][k];
}
