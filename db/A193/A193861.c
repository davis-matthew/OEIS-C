/*
 * A193861  Mirror of the triangle A193860.
 * https://oeis.org/A193861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = w(r, r-(n - r(r+1)/2)) where r is the unique row index
 *            with r(r+1)/2 ≤ n < (r+1)(r+2)/2. The triangle w(r,k) is defined
 *            by w(0,0) = 1, w(r,0) = 1, w(r,r) = 2*w(r-1,r-1) + 3^r, w(r,k) =
 *            w(r-1,k) + 2*w(r-1,k-1) (0 < k < r)
 * Range:     u8 n <= 14, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard includes for memory allocation. */
#include <stdlib.h>

/* Cached rows of triangle w(r,k). */
static seq_t **_row_cache = NULL;    /* pointer to array of row pointers   */
static size_t _cached_rows = 0;      /* number of rows currently cached   */
static seq_t _power3 = (seq_t)1;     /* 3^r for the last computed row     */

/* Ensure that rows 0..need are cached. */
static void _ensure_rows(idx_t need)
{
    while (_cached_rows <= (size_t)need) {
        /* First row: w(0,0)=1. */
        if (_cached_rows == 0) {
            _row_cache = (seq_t **)malloc(sizeof(seq_t *));
            if (_row_cache == NULL) exit(1);
            _row_cache[0] = (seq_t *)malloc(sizeof(seq_t));
            if (_row_cache[0] == NULL) exit(1);
            _row_cache[0][0] = (seq_t)1;
            _power3 = (seq_t)1;          /* 3^0 */
            _cached_rows = 1;
            continue;
        }

        /* Compute next row r = _cached_rows. */
        idx_t r = _cached_rows;
        seq_t *prev = _row_cache[r - 1];
        seq_t *curr = (seq_t *)malloc(sizeof(seq_t) * (r + 1));
        if (curr == NULL) exit(1);

        /* w(r,0) = 1. */
        curr[0] = (seq_t)1;

        /* Middle entries. */
        for (size_t k = 1; k < (size_t)r; ++k) {
            curr[k] = prev[k] + (seq_t)2 * prev[k - 1];
        }

        /* Update power of three: 3^r. */
        _power3 = _power3 * (seq_t)3;

        /* Last entry w(r,r). */
        curr[r] = (seq_t)2 * prev[r - 1] + _power3;

        /* Append the new row to the cache. */
        seq_t **new_cache = (seq_t **)
            realloc(_row_cache, sizeof(seq_t *) * (r + 1));
        if (new_cache == NULL) exit(1);
        _row_cache = new_cache;
        _row_cache[r] = curr;
        ++_cached_rows;
    }
}

/* Find the row index r that contains the linear index n. */
static inline idx_t _find_row(idx_t n)
{
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 <= n) {
        ++r;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Determine the row and column within that row. */
    idx_t r = _find_row(n);
    idx_t row_start = (r * (r + 1)) / 2;          /* triangular number */
    idx_t j = n - row_start;                      /* position in the row */
    idx_t k = r - j;                               /* mirror index within w(r,·) */

    /* Make sure the needed rows are present. */
    _ensure_rows(r);

    /* Return the requested triangle entry. */
    return _row_cache[(size_t)r][(size_t)k];
}
