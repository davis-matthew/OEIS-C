/*
 * A117975  Triangle where a(1,1)=1; a(n,m) = number of positive integers
 *          which are missing from row (n-1) of the triangle, are <= n and are
 *          coprime to m.
 * https://oeis.org/A117975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 111, u16 n <= 111, u32 n <= 111, u64 n <= 111,
 *            u128 n <= 111, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A117975: triangle where a(1,1)=1; a(n,m) = number of positive
   integers missing from row (n-1) that are <= n and coprime to m. */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd.h"

/* Cached triangle rows; rows[i] points to array of length i+1
   representing row (i+1). */
static seq_t **_rows = NULL;
static size_t _rows_cap = 0;
static size_t _rows_max = 0;          /* number of rows currently built */

/* Presence marker for numbers 1..N.  _present[i]==true iff i is
   present in the previous row being processed. */
static bool *_present = NULL;
static size_t _present_cap = 0;

/* Ensure the presence array can accommodate numbers up to cap-1. */
static void _ensure_present(size_t cap)
{
    if (_present_cap >= cap)
        return;
    size_t new_cap = (_present_cap == 0) ? 8 : _present_cap * 2;
    while (new_cap < cap)
        new_cap *= 2;
    bool *tmp = (bool *)realloc(_present, new_cap * sizeof(bool));
    if (!tmp)
        return; /* allocation failure - give up silently; future calls will
                    just use whatever is available */
    memset(tmp + _present_cap, 0, (new_cap - _present_cap) * sizeof(bool));
    _present = tmp;
    _present_cap = new_cap;
}

/* Build rows up to and including targetRow (1‑based). */
static void _build_rows(size_t targetRow)
{
    if (targetRow <= _rows_max)
        return;

    /* Grow rows array if necessary. */
    if (_rows_cap < targetRow) {
        size_t new_cap = (_rows_cap == 0) ? 8 : _rows_cap * 2;
        while (new_cap < targetRow)
            new_cap *= 2;
        seq_t **tmp = (seq_t **)realloc(_rows, new_cap * sizeof(seq_t *));
        if (!tmp)
            return; /* allocation failure */
        _rows = tmp;
        _rows_cap = new_cap;
    }

    /* Ensure presence array can hold numbers up to targetRow. */
    _ensure_present(targetRow + 1);

    /* Build new rows. */
    for (size_t r = _rows_max + 1; r <= targetRow; ++r) {
        seq_t *row = (seq_t *)malloc(r * sizeof(seq_t));
        if (!row)
            continue; /* OOM – skip this row; future calls will miss it */

        if (r == 1) {
            row[0] = (seq_t)1;
        } else {
            seq_t *prev = _rows[r - 2];            /* previous row */
            size_t prev_len = r - 1;

            /* Mark numbers from previous row that are <= r. */
            for (size_t i = 0; i < prev_len; ++i) {
                seq_t val = prev[i];
                if (val <= (seq_t)r)
                    _present[(size_t)val] = true;
            }

            /* Compute entries of current row. */
            for (size_t m = 1; m <= r; ++m) {
                seq_t cnt = 0;
                for (size_t k = 1; k <= r; ++k) {
                    if (!_present[k] && gcd((seq_t)k, (seq_t)m) == (seq_t)1)
                        ++cnt;
                }
                row[m - 1] = cnt;
            }

            /* Clear presence marks for next iteration. */
            for (size_t i = 0; i < prev_len; ++i) {
                seq_t val = prev[i];
                if (val <= (seq_t)r)
                    _present[(size_t)val] = false;
            }
        }

        _rows[r - 1] = row;
    }
    _rows_max = targetRow;
}

seq_t a(idx_t n)
{
    if (n == (idx_t)0)                    /* outside the offset */
        return (seq_t)0;

    /* Find the row that contains the linear index n. */
    seq_t row = 1;
    seq_t tri = 1;
    while (tri < n) {
        ++row;
        tri += row;                      /* next triangular number */
    }

    /* Convert to 0‑based indices for arrays. */
    size_t rownum = (size_t)row;          /* 1‑based row number */
    size_t col    = (size_t)(n - ((rownum - 1) * rownum) / 2); /* 1‑based col */

    _build_rows(rownum);                 /* ensure row is built */
    seq_t *rowptr = _rows[rownum - 1];
    if (!rowptr)
        return (seq_t)0;                 /* safety fallback */

    return rowptr[col - 1];
}
