/*
 * A108045  Triangle read by rows: lower triangular matrix obtained by
 *          inverting the lower triangular matrix in A108044.
 * https://oeis.org/A108045
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy
 * Formula:   T(n,k) = T(n‑1,k‑1) – T(n‑2,k); Initial values: T(0,0)=1,
 *            T(1,0)=0, T(1,1)=1, T(2,0)=−2. The sequence a(n) is the triangle
 *            read by rows: a(n) = T(r,c) where n = r(r+1)/2 + c, 0 ≤ c ≤ r
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>

static seq_t **triangle = NULL;   /* pointers to rows of the triangle */
static idx_t   rows_alloc = 0;    /* allocated number of row slots */
static idx_t   rows_used  = 0;    /* how many rows are actually computed */

/* Ensure that rows 0 … r exist in the array triangle. */
static void
ensure_row(idx_t r)
{
    /* Allocate more pointers if necessary. */
    if (rows_alloc <= r) {
        idx_t new_cap = rows_alloc ? rows_alloc * 2 : 4;
        while (new_cap <= r) new_cap *= 2;
        triangle = (seq_t**)realloc(triangle, new_cap * sizeof(seq_t*));
        rows_alloc = new_cap;
    }

    /* Compute rows from rows_used up to r. */
    for (idx_t i = rows_used; i <= r; ++i) {
        /* Allocate current row (length i+1). */
        seq_t *cur = (seq_t*)malloc((i + 1) * sizeof(seq_t));
        for (idx_t k = 0; k <= i; ++k) {
            seq_t val;
            if (i == 0) {                     /* row 0: 1 */
                val = (seq_t)1;
            } else if (i == 1) {              /* row 1: 0 , 1 */
                if (k == 0) val = (seq_t)0;
                else        val = (seq_t)1;
            } else if (i == 2) {              /* row 2: −2 , 0 , 1 */
                if (k == 0) val = (seq_t)-2;
                else if (k == 1) val = (seq_t)0;
                else            val = (seq_t)1;
            } else {                          /* recurrence for i ≥ 3 */
                seq_t t1 = (k > 0) ? triangle[i-1][k-1] : (seq_t)0;
                seq_t t2 = (k <= i-2) ? triangle[i-2][k] : (seq_t)0;
                val = t1 - t2;
            }
            cur[k] = val;
        }
        triangle[i] = cur;
        ++rows_used;
    }
}

/* Convert n to (row, col) in the triangle array. */
static void
row_col(idx_t n, idx_t *row, idx_t *col)
{
    /* Compute an approximate row using the inverse quadratic formula. */
    long double d = sqrtl(8.0L * (long double)n + 1.0L);
    idx_t r = (idx_t)((d - 1.0L) / 2.0L);

    /* Correct possible rounding errors. */
    while ((unsigned __int128)(r + 1) * (r + 2) / 2 <= n) ++r;
    while ((unsigned __int128)r * (r + 1) / 2 > n) --r;

    *row = r;
    *col = n - (unsigned __int128)r * (r + 1) / 2;
}

/* Public function returning the n‑th term of A108045. */
seq_t
a(idx_t n)
{
    idx_t r, c;
    row_col(n, &r, &c);

    /* Ensure the needed rows have been computed. */
    if (rows_used <= r) ensure_row(r);

    return triangle[r][c];
}
