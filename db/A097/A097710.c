/*
 * A097710  Lower triangular matrix T, read by rows, such that row (n) is
 *          formed from the sums of adjacent terms in row (n-1) of the matrix
 *          square T^2, with T(0,0)=1.
 * https://oeis.org/A097710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 54, u64 n <= 91, u128 n <= 189,
 *            big any n
 * Helpers:   seq_alloc_new_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   a(n) is generated from the lower‑triangular matrix T defined by
   T(0,0)=1 and, for n>=1,
      T(n,0) = Σ_{j=0}^{n-1} T(n-1,j) * T(j,0)                 (k=0)
      T(n,k) = Σ_{j=0}^{n-1} T(n-1,j) * (T(j,k-1)+T(j,k))   (1<=k<=n-1)
      T(n,n) = 1                                            (k=n)

   The triangle is read row by row from left to right:
      a(0)=T(0,0)=1
      a(1)=T(1,0)=1, a(2)=T(1,1)=1
      a(3)=T(2,0)=2, a(4)=T(2,1)=3, a(5)=T(2,2)=1
      a(6)=T(3,0)=7, a(7)=T(3,1)=13, a(8)=T(3,2)=7, a(9)=T(3,3)=1
      …
*/

#include <stdlib.h>
#include "../utils/seq_alloc_new_array.h"

static seq_t **T = NULL;                   /* array of rows */
static idx_t max_row_computed = (idx_t)-1;  /* -1 means no rows yet */
static size_t rows_cap = 0;                /* capacity of the rows array */

/* Ensure the rows array has room for at least `need` indices */
static void ensure_rows_cap(idx_t need)
{
    if (need < (idx_t)rows_cap) return;
    size_t new_cap = rows_cap ? rows_cap * 2 : 4;
    while ((idx_t)new_cap <= need) new_cap *= 2;
    seq_t **tmp = (seq_t**)realloc(T, new_cap * sizeof(seq_t*));
    T = tmp;
    rows_cap = new_cap;
}

/* Compute all rows of T up to (and including) row `r` */
static void compute_rows_up_to(idx_t r)
{
    if (max_row_computed != (idx_t)-1 && r <= max_row_computed) return;

    /* start from the first uncomputed row */
    idx_t start = (max_row_computed == (idx_t)-1) ? 0 : max_row_computed + 1;

    for (idx_t cur = start; cur <= r; ++cur) {
        idx_t len = cur + 1;
        seq_t *row = seq_alloc_new_array(len);

        /* last element is always 1 */
        row[cur] = (seq_t)1;

        /* compute the remaining elements */
        for (idx_t k = 0; k < cur; ++k) {
            seq_t sum = 0;
            if (k == 0) {
                /* T(cur,0) = Σ T(cur-1,j) * T(j,0) */
                for (idx_t j = 0; j < cur; ++j)
                    sum += T[cur-1][j] * T[j][0];
            } else {
                /* T(cur,k) = Σ T(cur-1,j) * (T(j,k-1)+T(j,k)) */
                for (idx_t j = 0; j < cur; ++j) {
                    seq_t part = 0;
                    if (j >= k)     part += T[j][k];
                    if (j >= k-1)   part += T[j][k-1];
                    sum += T[cur-1][j] * part;
                }
            }
            row[k] = sum;
        }

        ensure_rows_cap(cur);
        T[cur] = row;
        max_row_computed = cur;
    }
}

/* Entry point for the sequence. */
seq_t a(idx_t n)
{
    /* Find the row r that contains the term a(n). */
    idx_t r = 0;
    idx_t cum = 0;                 /* cumulative number of terms up to row r-1 */
    while (cum + (r + 1) <= n) {
        cum += r + 1;
        ++r;
    }

    compute_rows_up_to(r);

    idx_t pos = n - cum;           /* position inside row r */
    return T[r][pos];
}
