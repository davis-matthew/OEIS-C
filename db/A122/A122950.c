/*
 * A122950  Triangle T(n,k), 0 <= k <= n, read by rows, given by [0, 0, 1, -1,
 *          0, 0, 0, 0, 0, 0, ...] DELTA [1, 1, -1, 0, 0, 0, 0, 0, 0, ...]
 *          where DELTA is the operator defined in A084938.
 * https://oeis.org/A122950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row,col) where row,col are obtained from n by * row =
 *            floor((sqrt(8*n+1)-1)/2) * col = n - row*(row+1)/2 * T(n,k) is
 *            defined by * T(0,0)=1, T(2,1)=0, *
 *            T(n,k)=T(n-1,k-1)+T(n-2,k-1)+T(n-2,k-2) (k>=1), * T(n,k)=0 for
 *            k<0 or k>n. * The triangle values are cached row by row so that
 *            each call...
 * Range:     u8 n <= 74, u16 n <= 88, u32 n <= 88, u64 n <= 88, u128 n <= 88,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/fib.h"   /* provides seq_t fib(idx_t) */

static seq_t **T_rows_arr = NULL;   /* array of pointers to each row */
static size_t  T_rows_count = 0;    /* number of rows already allocated */
static size_t  T_rows_cap   = 0;    /* current capacity of the row pointer array */

/* Expand the row pointer array and allocate all rows up to max_row (inclusive).
   Each row is an array of seq_t of length r+1.  Zero values are assigned directly
   without calling constructors, which is safe for the seq_t types used in this
   project. */
static void ensure_rows(size_t max_row)
{
    while (T_rows_count <= max_row) {
        /* make room for the new row pointer if necessary */
        if (T_rows_count == T_rows_cap) {
            size_t new_cap = T_rows_cap ? T_rows_cap * 2 : 4;
            seq_t **new_ptr = (seq_t**)realloc(T_rows_arr,
                                               new_cap * sizeof(seq_t*));
            if (!new_ptr) return;                     /* low‑memory abort */
            T_rows_arr = new_ptr;
            T_rows_cap = new_cap;
        }

        size_t r   = T_rows_count;          /* index of the new row */
        size_t len = r + 1;                 /* number of elements in the row */

        seq_t *row = (seq_t*)malloc(len * sizeof(seq_t));
        if (!row) return;                    /* low‑memory abort */

        /* initialise the new row according to the recurrence */
        for (size_t k = 0; k < len; ++k) {
            if (r == 0) {                    /* first row, only T(0,0)=1 */
                row[0] = (seq_t)1;
                continue;
            }
            if (k == 0) {                    /* first column (except row 0) */
                row[k] = (seq_t)0;
                continue;
            }
            if (r == 2 && k == 1) {          /* special case T(2,1)=0 */
                row[k] = (seq_t)0;
                continue;
            }
            if (k == r) {                    /* diagonal entry is Fibonacci */
                row[k] = fib((idx_t)(r + 1));
                continue;
            }

            /* general recurrence: sum of up to three previous entries */
            seq_t sum = (seq_t)0;
            sum += T_rows_arr[r-1][k-1];      /* T(r-1, k-1) always exists */
            if (r >= 2) {
                sum += T_rows_arr[r-2][k-1];  /* T(r-2, k-1) */
                if (k >= 2)
                    sum += T_rows_arr[r-2][k-2];   /* T(r-2, k-2) */
            }
            row[k] = sum;
        }

        /* store the new row and update counters */
        T_rows_arr[T_rows_count] = row;
        ++T_rows_count;
    }
}

seq_t a(idx_t n)
{
    /* Determine which row contains the n‑th flattened element.
       The first element of row r starts at index r*(r+1)/2. */
    size_t r = 0;
    while ((r + 1)*(r + 2)/2 <= (size_t)n)
        ++r;

    ensure_rows(r);                      /* build all required rows */

    size_t offset = (size_t)n - r*(r + 1)/2;
    return T_rows_arr[r][offset];
}
