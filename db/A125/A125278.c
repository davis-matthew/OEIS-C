/*
 * A125278  Convolution triangle of A030266, which shifts left under
 *          self-COMPOSE.
 * https://oeis.org/A125278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, c) where T is the convolution triangle of A030266 *
 *            with recurrence: * T(0,0)=1; * T(n,0)= Σ_{j=0}^{n-1} T(j,0) *
 *            T(n-1, j); * T(n,k)= Σ_{j=0}^{n-k} T(j,0) * T(n-1-j, k-1) (k>0).
 *            * The sequence a(n) is obtained by reading the triangle row‑wise
 *            * (row0: 1; row1: 1,1; row2: 2,2,1; …)
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   _coords
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/_coords.h"

/* Static cache of computed rows */
static seq_t **_rows = NULL;
static idx_t   _row_count = 0;
static idx_t   _row_cap   = 0;

/* Ensure that rows up to 'upto' (inclusive) are computed */
static void _ensure_rows(idx_t upto)
{
    while (_row_count <= upto) {
        idx_t r = _row_count;
        seq_t *row = (seq_t *)malloc((size_t)(r + 1) * sizeof(seq_t));
        if (!row) abort();

        if (r == 0) {
            row[0] = 1;                 /* base case */
        } else {
            /* T(r,0) */
            seq_t sum0 = 0;
            for (idx_t j = 0; j < r; ++j)
                sum0 += _rows[j][0] * _rows[r - 1][j];
            row[0] = sum0;

            /* T(r,k) for k >= 1 */
            for (idx_t k = 1; k <= r; ++k) {
                seq_t sumk = 0;
                idx_t max_j = r - k;
                for (idx_t j = 0; j <= max_j; ++j)
                    sumk += _rows[j][0] * _rows[r - 1 - j][k - 1];
                row[k] = sumk;
            }
        }

        /* Grow the array of row pointers if needed */
        if (_row_count == _row_cap) {
            idx_t new_cap = (_row_cap ? _row_cap * 2 : 8);
            seq_t **new_rows = (seq_t **)realloc(_rows, (size_t)new_cap * sizeof(seq_t *));
            if (!new_rows) abort();
            _rows = new_rows;
            _row_cap = new_cap;
        }

        _rows[_row_count++] = row;
    }
}

/* Public function required by the specification */
seq_t a(idx_t n)
{
    idx_t r, c;
    _coords(n, &r, &c);
    _ensure_rows(r);
    return _rows[r][c];
}
