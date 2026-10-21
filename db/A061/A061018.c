/*
 * A061018  Triangle: a(n,m) = number of permutations of (1,2,...,n) with one
 *          or more fixed points in the m first positions.
 * https://oeis.org/A061018
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the nth term of the triangle of permutations * with at
 *            least one fixed point among the first m positions, * flattened
 *            row by row: a(1) = 1, a(2) = 1, a(3) = 1, a(4) = 2, …
 * Range:     u8 n <= 17, u16 n <= 37, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   _row_of_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/_row_of_index.h"

static idx_t   _max_row = 0;
static seq_t * _fact    = NULL;   /* factorial array, index 0..N   */
static seq_t **_T       = NULL;   /* 2‑D table for T(n,m), 0≤n,m≤N */

/* Ensure factorials and T table are available up to at least 'rows' */
static void _ensure_tables(idx_t rows)
{
    if (rows <= _max_row) return;

    /* resize factorial array */
    _fact = (seq_t *)realloc(_fact, (rows + 1) * sizeof(seq_t));
    /* resize row pointers */
    _T = (seq_t **)realloc(_T, (rows + 1) * sizeof(seq_t *));
    /* initialise factorial[0] */
    if (_max_row == 0) _fact[0] = (seq_t)1;

    /* compute missing factorials */
    for (idx_t i = _max_row + 1; i <= rows; ++i)
        _fact[i] = _fact[i - 1] * (seq_t)i;

    /* allocate each required row for T and compute it */
    for (idx_t n = _max_row; n <= rows; ++n)
        _T[n] = (seq_t *)realloc(_T[n], (n + 1) * sizeof(seq_t));

    /* compute T values up to n = rows-2 */
    if (rows >= 2) {
        for (idx_t n = 0; n <= rows - 2; ++n) {
            _T[n][0] = (seq_t)n * _fact[n];
            for (idx_t m = 1; m <= n; ++m)
                _T[n][m] = _T[n][m - 1] - _T[n - 1][m - 1];
        }
    }

    _max_row = rows;
}

/* Return a(row,col) in the triangle */
static seq_t _a_row_col(idx_t row, idx_t col)
{
    if (row == 0) return (seq_t)0;   /* safety, never used */
    seq_t fact_val = _fact[row - 1];  /* (row-1)! */
    if (col == 1) return fact_val;   /* first column is factorial */

    idx_t n     = row - 2;           /* we need T(row-2, k) for k <= col-2 */
    idx_t kmax  = col - 2;
    seq_t sum_T = (seq_t)0;
    for (idx_t k = 0; k <= kmax; ++k)
        sum_T += _T[n][k];
    return fact_val + sum_T;
}

/* Public function ---------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* sequence is 1‑based */

    /* find row r and column c for this index */
    idx_t r = _row_of_index(n);
    idx_t start = 1 + r * (r - 1) / 2;   /* first index in this row */
    idx_t c = n - start + 1;              /* column number (1‑based) */

    _ensure_tables(r);
    return _a_row_col(r, c);
}
