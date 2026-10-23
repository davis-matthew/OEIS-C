/*
 * A011971  Aitken's array: triangle of numbers {a(n,k), n >= 0, 0 <= k <= n}
 *          read by rows, defined by a(0,0)=1, a(n,0) = a(n-1,n-1), a(n,k) =
 *          a(n,k-1) + a(n-1,k-1).
 * https://oeis.org/A011971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabl, nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 51, u32 n <= 128, u64 n <= 339,
 *            u128 n <= 937, big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A011971: Aitken's array triangle read by rows.  
   Recurrence: a(0,0)=1; a(n,0)=a(n-1,n-1); a(n,k)=a(n,k-1)+a(n-1,k-1).  
   The function returns the n‑th element of the flattened triangle, where  
   a(0)=1, a(1)=1, a(2)=2, a(3)=2, a(4)=3, a(5)=5, … */
#include <stdlib.h>
#include <stddef.h>
#include "../utils/alloc_seq.h"

static seq_t **_rows = NULL;      /* cached rows of the triangle */
static idx_t  _row_cnt = 0;       /* number of rows stored */
static idx_t  _row_cap = 0;       /* capacity of the _rows array */

/* Return the nth element of the flattened A011971 triangle. */
seq_t a(idx_t n)
{
    /* Find the row `r` and column `k` for the element with index `n`. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t prev_terms = r * (r + 1) / 2;   /* elements before row r */
    idx_t k = n - prev_terms;            /* column inside row r (0‑based) */

    /* Generate all rows up to `r` on demand, caching them. */
    while (_row_cnt <= r) {
        /* Ensure capacity for the array of row pointers. */
        if (_row_cnt >= _row_cap) {
            size_t new_cap = (_row_cap == 0) ? 1 : _row_cap * 2;
            seq_t **tmp = (seq_t**)realloc(_rows, new_cap * sizeof(seq_t *));
            if (!tmp)
                abort();                     /* out of memory */
            _rows = tmp;
            _row_cap = (idx_t)new_cap;
        }

        /* Allocate the new row. */
        size_t len = (size_t)(_row_cnt + 1);
        seq_t *row = alloc_seq(len);
        if (!row)
            abort();                         /* allocation failed */

        if (_row_cnt == 0) {
            /* First row contains only a(0,0)=1. */
            row[0] = (seq_t)1;
        } else {
            seq_t *prev = _rows[_row_cnt - 1];
            /* a(n,0) = a(n-1,n-1). */
            row[0] = prev[_row_cnt - 1];
            for (size_t i = 1; i < len; ++i) {
                /* a(n,k) = a(n,k-1)+a(n-1,k-1). */
                row[i] = row[i - 1] + prev[i - 1];
            }
        }

        _rows[_row_cnt] = row;
        ++_row_cnt;
    }

    /* Return the cached value. */
    return _rows[(size_t)r][(size_t)k];
}
