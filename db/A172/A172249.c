/*
 * A172249  Triangle, read by rows, given by [0,1/3,-1/3,0,0,0,0,0,0,0,...]
 *          DELTA [3,-1/3,1/3,0,0,0,0,0,0,0,...] where DELTA is the operator
 *          defined in A084938.
 * https://oeis.org/A172249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 73, u32 n <= 73, u64 n <= 73, u128 n <= 73,
 *            big any n
 * Helpers:   seq_alloc_array, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) is the flattened A172249 triangle.  The triangle T(n,k) is defined by:
 *
 *      T(0,0) = 1,
 *      T(n,k) = 3*T(n-1,k-1) + T(n-2,k-1) – T(n-2,k-2)  for n≥1, k≥0,
 *               T(n,k) = 0 if k>n or k<0.
 *
 *  a(n) is the element T(r,k) where r is the row containing the
 *  index n (rows are counted from 0) and k is the column within that row.
 *  The mapping is: r = ⌊(√(8n+1)–1)/2⌋,  k = n – r*(r+1)/2.
 *
 *  The function memoises all computed rows in a dynamic array so
 *  that repeated calls are fast.  No heavyweight search or prime
 *  sieve is required, and the routine works for all values that
 *  fit in seq_t (including the arbitrary‑precision build).
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/seq_one.h"

static seq_t **_rows      = NULL;   /* array of pointers to each row   */
static idx_t   _rows_cap  = 0;       /* current allocated capacity      */
static idx_t   _max_row   = (idx_t)-1; /* highest row fully computed */

/* Grow the array of row pointers if necessary */
static void _grow_row_ptrs(idx_t new_cap)
{
    if (_rows_cap < new_cap) {
        seq_t **tmp = (seq_t**)realloc(_rows, new_cap * sizeof(seq_t*));
        if (!tmp) abort();
        /* initialise new slots to NULL */
        for (idx_t i = _rows_cap; i < new_cap; ++i) tmp[i] = NULL;
        _rows     = tmp;
        _rows_cap = new_cap;
    }
}

/* Ensure that rows 0 … target are computed and stored in _rows */
static void _ensure_rows(idx_t target)
{
    if (_max_row != (idx_t)-1 && _max_row >= target) return;

    if (_rows == NULL) _grow_row_ptrs((size_t)(target + 1) < 8 ? 8 : target + 1);

    /* Row 0 */
    if (_max_row == (idx_t)-1) {
        _rows[0] = seq_alloc_array(1);
        _rows[0][0] = seq_one();
        _max_row = 0;
    }

    for (idx_t r = _max_row + 1; r <= target; ++r) {
        _rows[r] = seq_alloc_array((size_t)(r + 1));
        for (idx_t k = 0; k <= r; ++k) {
            if (k == 0) {
                _rows[r][k] = 0;
            } else {
                seq_t val = 3 * _rows[r-1][k-1];
                if (r >= 2 && k-1 <= r-2) val += _rows[r-2][k-1];
                if (k >= 2 && k-2 <= r-2) val -= _rows[r-2][k-2];
                _rows[r][k] = val;
            }
        }
    }
    _max_row = target;
}

/* Public function required by the framework */
seq_t a(idx_t n)
{
    /* Find the row r that contains the index n.  Solve
       (r+1)(r+2)/2 <= n < (r+2)(r+3)/2  */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) ++r;
    idx_t k = n - (r * (r + 1)) / 2;

    _ensure_rows(r);
    return _rows[r][k];
}
