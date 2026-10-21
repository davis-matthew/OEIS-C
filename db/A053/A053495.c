/*
 * A053495  Triangle formed by coefficients of numerator polynomials defined
 *          by iterating f(u,v) = 1/u - x*v applied to a list of elements
 *          {1,2,3,4,...}.
 * https://oeis.org/A053495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, easy, nice
 * Formula:   a(n) is the n-th entry of the triangle of coefficients of the
 *            polynomials defined by p0 = -1, p1 = 1 - x, p_n = n*x*p_{n-1} +
 *            p_{n-2} for n >= 2 The triangle is read row‑wise, but a(0) is
 *            defined as 1 by the OEIS offset (the first coefficient of p0 is
 *            −1)
 * Range:     i8 n <= 23, i16 n <= 41, i32 n <= 49, i64 n <= 49, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Global storage for the computed rows of the triangle */
static size_t _row_cap = 0;
static seq_t **_rows    = NULL;
static idx_t  _row_cnt  = 0;

/* Ensure that all rows 0 … upto are computed and stored. */
static void _ensure_rows(idx_t upto)
{
    if (_row_cap == 0) {
        _row_cap = 4;
        _rows = (seq_t **)malloc(_row_cap * sizeof(seq_t *));
    }

    while (_row_cnt <= upto) {
        /* grow the array of row pointers if necessary */
        if (_row_cnt >= _row_cap) {
            _row_cap = _row_cap * 2 + 2;
            _rows = (seq_t **)realloc(_rows, _row_cap * sizeof(seq_t *));
        }

        if (_row_cnt == 0) {                     /* row 0 : p0 = -1 */
            seq_t *row0 = (seq_t *)malloc(sizeof(seq_t) * 1);
            row0[0] = (seq_t)(-1);
            _rows[0] = row0;
            _row_cnt  = 1;
        }
        else if (_row_cnt == 1) {                /* row 1 : p1 = 1 - x */
            seq_t *row1 = (seq_t *)malloc(sizeof(seq_t) * 2);
            row1[0] = (seq_t)1;
            row1[1] = (seq_t)(-1);
            _rows[1] = row1;
            _row_cnt  = 2;
        }
        else {                                   /* general case */
            idx_t r      = _row_cnt;              /* index of the new row */
            seq_t *prev1 = _rows[r - 1];          /* length r   */
            seq_t *prev2 = _rows[r - 2];          /* length r-1 */
            seq_t *cur   = (seq_t *)malloc(sizeof(seq_t) * (r + 1));

            /* c[0] = prev2[0] */
            cur[0] = prev2[0];

            if (r >= 2) {
                /* j = 1 … r-2 */
                for (idx_t j = 1; j < r - 1; ++j) {
                    cur[j] = (seq_t)r * prev1[j - 1] + prev2[j];
                }
                /* j = r-1 : no term from prev2 */
                cur[r - 1] = (seq_t)r * prev1[r - 2];
            }
            /* j = r : only the shifted term */
            cur[r] = (seq_t)r * prev1[r - 1];

            _rows[r] = cur;
            ++_row_cnt;
        }
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0u)          /* offset 0: a(0) is defined as 1 */
        return (seq_t)1;

    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    for (;;) {
        idx_t nxt = r + 1;
        if ((seq_t)nxt * (nxt + 1) / 2 <= n)
            r = nxt;
        else
            break;
    }

    _ensure_rows(r);

    idx_t offset = r * (r + 1) / 2;   /* first index in row r */
    idx_t c      = n - offset;        /* column within the row */

    return _rows[r][c];
}
