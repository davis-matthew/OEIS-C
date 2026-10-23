/*
 * A166960  Triangle T(n, k) read by rows: T(n, k)= (m*n-m*k+1)*T(n-1, k-1) +
 *          k*(m*k-(m-1))*T(n-1, k) where m = 1.
 * https://oeis.org/A166960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 30, u32 n <= 59, u64 n <= 141,
 *            u128 n <= 325, big any n
 * Helpers:   _mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: T(n,k) = (n-k+1)*T(n-1,k-1) + k^2*T(n-1,k);  T(1,1)=1
   a(n) is the row‑major flattening of the triangle.  For n ≥ 1,
   a(n) = T(r,k) where r is the unique integer with r(r+1)/2 ≥ n
   and k = n – r(r-1)/2.  The function memoises the computed rows
   so that calls in any order are efficient. */

#include <stdlib.h>
#include "../utils/_mul.h"

/* static cache of triangle rows, indexed 1..max_row (row 0 unused) */
static seq_t **_rows = NULL;        /* _rows[1] .. _rows[max_row] */
static idx_t  _max_row = 0;         /* highest row that has been built */

/* public function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;            /* defined only for n ≥ 1 */

    /* find r such that r(r+1)/2 ≥ n, r minimal */
    idx_t r = 1;
    while ((idx_t)r * ((idx_t)r + 1) / 2 < n)
        ++r;

    /* column index k (1‑based) */
    idx_t k = n - (idx_t)r * ((idx_t)r - 1) / 2;

    /* build rows up to r if necessary */
    if ((size_t)r > (size_t)_max_row) {
        /* grow the outer array of pointers */
        seq_t **new_rows = (seq_t **)realloc(_rows,
                                   (size_t)(r + 1) * sizeof(seq_t *));
        if (!new_rows)
            return (seq_t)0;        /* allocation failure */
        _rows = new_rows;

        /* compute all new rows */
        for (idx_t i = (idx_t)_max_row + 1; i <= r; ++i) {
            _rows[i] = (seq_t *)malloc((size_t)(i + 1) * sizeof(seq_t));
            if (! _rows[i])
                return (seq_t)0;    /* allocation failure */

            if (i == 1) {           /* base case: T(1,1)=1 */
                _rows[1][1] = (seq_t)1;
            } else {
                for (size_t col = 1; col <= i; ++col) {
                    seq_t left  = 0;
                    seq_t right = 0;

                    if (col > 1) {
                        /* (i-col+1) * T(i-1, col-1) */
                        seq_t prev = _rows[i - 1][col - 1];
                        left = _mul((seq_t)(i - col + 1), prev);
                    }

                    if (col <= i - 1) {
                        /* col^2 * T(i-1, col) */
                        seq_t prev = _rows[i - 1][col];
                        seq_t k_sq = _mul((seq_t)col, (seq_t)col);
                        right = _mul(k_sq, prev);
                    }

                    _rows[i][col] = left + right;
                }
            }
        }
        _max_row = r;
    }

    return _rows[r][k];
}
