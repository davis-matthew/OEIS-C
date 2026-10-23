/*
 * A172250  Triangle, read by rows, given by [0,1,-1,0,0,0,0,0,0,0,...] DELTA
 *          [1,-1,1,0,0,0,0,0,0,0,...] where DELTA is the operator defined in
 *          A084938.
 * https://oeis.org/A172250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   T(n,k) = T(n‑1,k‑1) + T(n‑2,k‑1) – T(n‑2,k‑2), T(0,0) = 1. The
 *            sequence a(n) is the flattened triangle read by rows: row =
 *            ⌊(√(8n+1) – 1)/2⌋ , col = n – row(row+1)/2, a(n) = T(row,col).
 *            Initial values: a(0)=1, a(1)=0, a(2)=1, a(3)=0, a(4)=1, a(5)=0,
 *            a(6)=0, a(7)=0, a(8)=2, a(9)=–1, …
 * Range:     i8 n <= 90, i16 n <= 90, i32 n <= 90, i64 n <= 90, i128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <math.h>

/* local helpers ----------------------------------------------------------- */
static idx_t _row_of(idx_t n);
static void _ensure_rows(idx_t needed);

/* storage ----------------------------------------------------------- */
static seq_t **_rows = NULL;      /* pointer to each row (index 0…r)     */
static idx_t  _rows_cap = 0;      /* number of rows currently allocated */

/* Find the row number r that contains flattened index n. */
static inline idx_t _row_of(idx_t n)
{
    idx_t low = 0, high = (idx_t)(sqrtl((long double)(8.0L * (long double)n + 1.0L)) + 2.0L);
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        seq_t val = (seq_t)mid * ((seq_t)mid + 1) / 2;
        if (val == (seq_t)n)
            return mid;
        if (val < (seq_t)n)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;   /* high is maximal r with r(r+1)/2 ≤ n */
}

/* Build rows up to "needed" (inclusive).  The triangle is accumulated
   incrementally, each row derived from the recurrence above. */
static void _ensure_rows(idx_t needed)
{
    if (_rows_cap > needed)
        return;   /* nothing to do */

    /* Allocate or grow the array of row pointers. */
    idx_t new_cap = needed + 1;
    _rows = (seq_t**)realloc(_rows, new_cap * sizeof(seq_t *));
    for (idx_t i = _rows_cap; i <= needed; ++i) {
        seq_t *row = (seq_t*)calloc((size_t)(i + 1), sizeof(seq_t));
        if (!row)
            abort();  /* allocation failure */

        if (i == 0) {                /* base row */
            row[0] = 1;
        } else {                     /* compute row i by recurrence */
            for (idx_t k = 0; k <= i; ++k) {
                seq_t t1 = 0, t2 = 0, t3 = 0;
                if (k >= 1) {
                    t1 = _rows[i - 1][k - 1];
                }
                if (i >= 2 && k >= 1 && k <= i - 1) {
                    t2 = _rows[i - 2][k - 1];
                }
                if (i >= 2 && k >= 2 && k <= i) {
                    t3 = _rows[i - 2][k - 2];
                }
                row[k] = t1 + t2 - t3;
            }
        }
        _rows[i] = row;
    }
    _rows_cap = new_cap;
}

/* required public function ------------------------------------------------ */
seq_t a(idx_t n)
{
    idx_t r = _row_of(n);
    seq_t offset = (seq_t)r * ((seq_t)r + 1) / 2;
    idx_t k = (idx_t)((seq_t)n - offset);

    _ensure_rows(r);      /* make sure the triangle is present */

    return _rows[r][k];
}
