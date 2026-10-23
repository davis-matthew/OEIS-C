/*
 * A136247  Triangle T(n,k) read by rows: coefficient [x^k] of the polynomial
 *          h(n,x) with h(0,x)=1, h(1,x)=1-x and recursively h(n,x) = 1 + n
 *          -(1-x)*(1-h(n-1,x)) - n*h(n-2,x).
 * https://oeis.org/A136247
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, tabl, sign
 * Formula:   a(n) is the n‑th coefficient read by rows of the triangle T(n,k)
 *            defined by the polynomial recursion h(0,x)=1, h(1,x)=1-x,
 *            h(n,x)=1 + n -(1-x)*(1-h(n-1,x)) - n*h(n-2,x). Coefficient
 *            recurrence for T(n,k) (0≤k≤n) is T(0,0)=1, T(1,0)=1, T(1,1)=-1,
 *            T(n,0)=n+T(n-1,0)-n*T(n-2,0), T(n,1)=T(n-1,1)-n*T(n...
 * Range:     i8 n <= 29, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   _row_and_col
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/_row_and_col.h"

/* Cached rows of the triangle: rows[i] points to an array of length i+1.
   Memory grows as needed.  The array of pointers itself is also grown. */
static seq_t **_rows = NULL;
static idx_t   _max_row = (idx_t)-1;

/* Ensure rows up to 'up' are available.  Allocates new rows using the
   coefficient recurrence and stores them in the cache. */
static void _ensure_rows(idx_t up)
{
    if (_rows == NULL) {
        _rows = (seq_t**)malloc((up + 1) * sizeof(seq_t*));
        _max_row = (idx_t)-1;
    } else if (up > _max_row) {
        _rows = (seq_t**)realloc(_rows, (up + 1) * sizeof(seq_t*));
    }

    for (idx_t n = _max_row + 1; n <= up; ++n) {
        /* Allocate storage for row n */
        seq_t *row_n = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        if (n == 0) {
            row_n[0] = (seq_t)1;
        } else if (n == 1) {
            row_n[0] = (seq_t)1;
            row_n[1] = (seq_t)-1;
        } else {
            seq_t *prev1 = _rows[n - 1];      /* h_{n-1} */
            seq_t *prev2 = _rows[n - 2];      /* h_{n-2} */
            seq_t kn = (seq_t)n;

            /* T(n,0) */
            row_n[0] = kn + prev1[0] - kn * prev2[0];

            /* T(n,1) */
            seq_t term1 = prev1[1];           /* always exists for n≥2 */
            seq_t term2 = (n >= 3 ? prev2[1] : (seq_t)0);
            row_n[1] = term1 - kn * term2 + ((seq_t)1 - prev1[0]);

            /* Remaining k≥2 */
            for (idx_t k = 2; k <= n; ++k) {
                seq_t t1 = (k <= n - 1) ? prev1[k] : (seq_t)0;
                seq_t t2 = (k <= n - 2) ? prev2[k] : (seq_t)0;
                row_n[k] = t1 - kn * t2 - prev1[k - 1];
            }
        }
        _rows[n] = row_n;
        _max_row = n;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)   /* trivial case */
        return (seq_t)1;

    idx_t row, col;
    _row_and_col(n, &row, &col);
    _ensure_rows(row);
    return _rows[row][col];
}
