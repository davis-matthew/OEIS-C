/*
 * A116188  Triangle T(n,k), 0<=k<=n : T(n,k)is smallest number such that
 *          T(n,k)>=T(n-1,k-1), T(n,k)>=T(n-1,k), T(n,k)and
 *          T(n-1,k-1)+T(n-1,k) have the same parity, T(0,0)=1 .
 * https://oeis.org/A116188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 96, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A116188 – triangle of minimal numbers with parity constraint.
    The sequence a(n) is the T(r,k) entry where n = r*(r+1)/2 + k.
    T(0,0)=1, and for r>0, 0<=k<=r:
          T(r,k) = max( T(r-1,k-1), T(r-1,k) )
          if parity( max ) != parity( T(r-1,k-1)+T(r-1,k) ) then T(r,k)++ */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

/* Interpret seq_t and idx_t as per build. */

static seq_t **rows = NULL;           /* pointers to rows of the triangle    */
static size_t rows_cap = 0;           /* allocated capacity for rows array   */
static idx_t max_row = (idx_t)-1;     /* highest computed row                 */

/* Return r*(r+1)/2 as idx_t.  Use seq_t arithmetic to avoid overflow. */
static inline idx_t offset_of(idx_t r)
{
    return (idx_t)((seq_t)r * (r + 1) / 2);
}

/* Ensure we have computed all rows up to r (inclusive).  Uses the recurrence
   described above. */
static void ensure_rows_up_to(idx_t r)
{
    /* allocate rows array if first time */
    if (rows == NULL) {
        rows_cap = 1;
        rows = (seq_t **)calloc(rows_cap, sizeof(seq_t *));
    }

    /* initialise row 0 if not done yet */
    if (max_row == (idx_t)-1) {
        rows[0] = (seq_t *)malloc(sizeof(seq_t));
        rows[0][0] = 1;
        max_row = 0;
    }

    while (max_row < r) {
        idx_t new_row = max_row + 1;

        /* grow rows array if needed */
        if ((size_t)new_row >= rows_cap) {
            size_t new_cap = rows_cap * 2;
            while (new_cap <= (size_t)new_row) new_cap *= 2;
            rows = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
            rows_cap = new_cap;
        }

        /* allocate and compute this new row */
        seq_t *arr = (seq_t *)malloc((new_row + 1) * sizeof(seq_t));
        for (idx_t k = 0; k <= new_row; ++k) {
            seq_t left  = 0;
            seq_t right = 0;

            if (k > 0)          left  = rows[new_row - 1][k - 1];
            if (k < new_row)    right = rows[new_row - 1][k];

            seq_t low = (left > right) ? left : right;
            seq_t parity = (left + right) & 1;

            if ((low & 1) != parity) low += 1;
            arr[k] = low;
        }
        rows[new_row] = arr;
        max_row = new_row;
    }
}

/* Given a linear index n, find the corresponding row r.  Because n is
   small (~10⁴) for the intended tests, a simple linear search is fast. */
static inline idx_t row_for(idx_t n)
{
    idx_t r = 0;
    while (offset_of(r + 1) <= n) ++r;
    return r;
}

/* Public function required by the exercise:  returns a(n). */
seq_t a(idx_t n)
{
    idx_t r = row_for(n);
    idx_t k = n - offset_of(r);

    ensure_rows_up_to(r);
    return rows[r][k];
}
