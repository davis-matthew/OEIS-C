/*
 * A118344  Pendular Catalan triangle, read by rows, where row n is formed
 *          from row n-1 by the recurrence: if n > 2k, T(n,k) = T(n,n-k) +
 *          T(n-1,k), else T(n,k) = T(n,n-1-k) - T(n-1,k) - T(n-1,k+1), for
 *          n>=k>=0, with T(n,0)=1 and T(n,n)=0^n.
 * https://oeis.org/A118344
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 238, u32 n <= 755, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(n) = T(r,k) where
 *   r is the largest integer with r*(r+1)/2 ≤ n
 *   k = n - r*(r+1)/2
 *   T(0,0)=1, T(n,0)=1, T(n,n)=0^n (0 for n>0)
 *   For n≥k≥1:
 *     if (n > 2*k)  T(n,k) = T(n,n-k) + T(n-1,k)
 *     else           T(n,k) = T(n,n-1-k) - T(n-1,k) - T(n-1,k+1)
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t **_row_vals      = NULL;   /* pointers to each row's values */
static bool **_row_computed   = NULL;   /* flags indicating computed entries */
static size_t _rows_cap       = 0;       /* capacity of the row-pointer arrays */
static idx_t  _rows_count     = 0;       /* number of rows currently allocated */

/* ensure that rows 0..r are allocated and initialised */
static void _ensure_rows_to(idx_t r)
{
    while (_rows_count <= r) {
        idx_t cur = _rows_count;
        size_t len = cur + 1;

        seq_t *row_vals = (seq_t*)malloc(len * sizeof(seq_t));
        bool  *row_comp = (bool *)malloc(len * sizeof(bool));
        if (!row_vals || !row_comp) return;          /* allocation failed – ignore */

        for (size_t i = 0; i < len; ++i) {
            row_vals[i] = 0;
            row_comp[i] = false;
        }

        /* base values */
        row_vals[0] = 1;
        row_comp[0] = true;
        if (cur > 0) {
            row_vals[cur] = 0;
            row_comp[cur] = true;
        }

        /* grow the row-pointer arrays if needed */
        if (_rows_count == _rows_cap) {
            size_t new_cap = _rows_cap ? _rows_cap * 2 : 1;
            seq_t **new_rows_val = (seq_t**)realloc(_row_vals, new_cap * sizeof(seq_t*));
            bool  **new_rows_com = (bool **)realloc(_row_computed, new_cap * sizeof(bool*));
            _row_vals      = new_rows_val;
            _row_computed  = new_rows_com;
            _rows_cap      = new_cap;
        }

        _row_vals[_rows_count]    = row_vals;
        _row_computed[_rows_count] = row_comp;
        ++_rows_count;
    }
}

/* recursive computation of T(n,k) with memoisation */
static seq_t _compute_T(idx_t n, idx_t k)
{
    if (k > n) return 0;                          /* out of bounds */

    _ensure_rows_to(n);                           /* guarantee row n is present */

    if (_row_computed[n][k])                      /* already computed */
        return _row_vals[n][k];

    seq_t val;

    if (n == 0) {                                 /* base 0,0 */
        val = 1;
    } else if (k == 0) {                          /* T(n,0)=1 */
        val = 1;
    } else if (k == n) {                          /* T(n,n)=0^n */
        val = (n == 0) ? 1 : 0;
    } else {
        if (n > 2*k) {
            /* T(n,k) = T(n,n-k) + T(n-1,k) */
            seq_t t1 = _compute_T(n, n - k);
            seq_t t2 = _compute_T(n - 1, k);
            val = t1 + t2;
        } else {
            /* T(n,k) = T(n,n-1-k) - T(n-1,k) - T(n-1,k+1) */
            seq_t t1 = _compute_T(n, n - 1 - k);
            seq_t t2 = _compute_T(n - 1, k);
            seq_t t3 = _compute_T(n - 1, k + 1);
            val = t1 - t2 - t3;
        }
    }

    _row_vals[n][k]    = val;
    _row_computed[n][k] = true;
    return val;
}

seq_t a(idx_t n)
{
    /* locate the row r containing index n */
    idx_t r = 0;
    idx_t offset = 0;
    while (offset + r + 1 <= n) {
        offset += r + 1;
        ++r;
    }

    /* column within the row */
    idx_t k = n - offset;

    /* compute T(r,k) if not already done */
    return _compute_T(r, k);
}
