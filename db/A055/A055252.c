/*
 * A055252  Triangle of partial row sums (prs) of triangle A055249.
 * https://oeis.org/A055252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 44, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   compute_row_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A055252 – Triangle of partial row sums (prs) of triangle A055249.
   The element a(n) is the (n)-th term in row‑major order of the
   triangle.  For each row r (starting at 0) the entries are
   a(r,0), a(r,1), … , a(r,r).
   Recurrences used:

   1.  a(r,r) = 1                                 (last column)
   2.  a(r,m+1) = a(r-1,m) + a(r-1,m+1)   for 0 ≤ m ≤ r-1
      (derived from the column recurrence of the OEIS entry)

   3.  a(r,0) = sum_{i=0}^r T(r,i)  where
          T(r,i) = sum_{k=0}^{r-i} (i+2)(i+3)···(i+r+2)
                    * (r-i)(r-i-1)···1 / (i+r+2)(i+r+1)···(i+3)
          Simplifying the hypergeometric sum gives the closed form

          a(r,0) = Σ_{i=0}^r term_i
          term_0 = 1
          term_{i+1} = term_i * (i+3) * (r-i) / ((i+1)^2)

          This is an O(r) computation and yields a(r,0) exactly.

   4.  For m≥1, all values are obtained from the recurrence (2).
   5.  A prefix sum of all a(j,0) (j < r) is kept; it equals a(r,1)
          because the column recurrence gives
          a(r,1) = Σ_{j=0}^{r-1} a(j,0).

   The algorithm pre‑computes rows incrementally up to the required
   row whenever a(n) is requested.  All arithmetic is performed
   with type seq_t, and no floating point operations are used.  The
   code is valid in both C11 and C++17; seq_t may be a builtin
   integer type or an arbitrary‑precision class (overloading the
   usual operators). */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/compute_row_zero.h"

static seq_t **rows = NULL;     /* triangle rows */
static size_t rows_cap = 0;     /* capacity of rows array */
static size_t rows_used = 0;    /* number of rows currently stored */
static seq_t sum_prefix0 = 0;   /* Σ a(j,0) for j < current maximum row */

/* ensure that rows 0 … rmax are computed */
static void ensure_rows(idx_t rmax)
{
    if (rmax < (idx_t)rows_used) return;

    /* expand the array of row pointers if necessary */
    size_t needed_cap = (size_t)rows_used;
    while (needed_cap <= (size_t)rmax) needed_cap = needed_cap ? needed_cap * 2 : 1;
    if (needed_cap > rows_cap) {
        seq_t **new_rows = (seq_t**)realloc(rows, needed_cap * sizeof(seq_t*));
        for (size_t i = rows_cap; i < needed_cap; ++i) new_rows[i] = NULL;
        rows = new_rows;
        rows_cap = needed_cap;
    }

    /* compute new rows incrementally */
    for (size_t cur = rows_used; cur <= (size_t)rmax; ++cur) {
        size_t len = cur + 1;
        seq_t *row = (seq_t*)malloc(len * sizeof(seq_t));

        /* a(cur,0) */
        seq_t a0 = compute_row_zero((idx_t)cur);
        row[0] = a0;

        /* a(cur,m) for m ≥ 1 via recurrence (2) */
        if (cur > 0) {
            for (size_t m = 0; m < cur; ++m) {
                seq_t left = rows[cur-1][m];
                seq_t right = (m + 1 < cur ? rows[cur-1][m+1] : (seq_t)0);
                row[m+1] = left + right;
            }
        }

        rows[(size_t)cur] = row;
        sum_prefix0 += a0;
        rows_used = (size_t)cur + 1;
    }
}

/* public function required by the exercise */
seq_t a(idx_t n)
{
    /* determine row r and column m for index n (row‑major order) */
    idx_t r = 0;
    /* smallest r such that r*(r+1)/2 > n gives r-1 as row number */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;  /* safe for idx_t */
    idx_t m = n - r * (r + 1) / 2;

    ensure_rows(r);
    return rows[(size_t)r][(size_t)m];
}
