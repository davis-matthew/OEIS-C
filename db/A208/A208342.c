/*
 * A208342  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A208343; see the Formula section.
 * https://oeis.org/A208342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the concatenation of the rows of the triangle T(n,k)
 *            defined by T(n,k) = T(n-1,k) + T(n-1,k-1) - T(n-2,k-1) +
 *            T(n-2,k-2), with initial rows T(1,0)=1, T(2,0)=1, T(2,1)=1,
 *            (rows are 1‑indexed, k runs from 0 to n-1). The sequence a(n)
 *            lists the entries row by row
 * Range:     u8 n <= 73, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*--- internal storage for triangle rows ---------------------------------*/
static seq_t **_rows = NULL;     /* pointer array: _rows[r] -> array of length r */
static size_t  _rows_cap = 0;    /* allocated capacity of _rows          */
static idx_t   _rows_cnt = 0;    /* number of rows actually computed     */

/* Ensure rows up to index n (1‑based) are computed */
static void _ensure_rows(idx_t n)
{
    if (n <= _rows_cnt) return;

    /* enlarge rows array if needed */
    if (_rows_cap < (size_t)(n + 1)) {
        size_t new_cap = _rows_cap ? _rows_cap * 2 : n + 1;
        seq_t **tmp = (seq_t **)realloc(_rows, new_cap * sizeof(seq_t *));
        if (!tmp) exit(EXIT_FAILURE); /* memory exhausted */
        _rows = tmp;
        _rows_cap = new_cap;
    }

    /* compute missing rows */
    for (idx_t r = _rows_cnt + 1; r <= n; ++r) {
        seq_t *row = (seq_t *)malloc((size_t)r * sizeof(seq_t));
        if (!row) exit(EXIT_FAILURE);

        if (r == 1) {
            row[0] = (seq_t)1;
        } else {
            seq_t *prev  = _rows[r - 1];          /* T(r-1, *) */
            seq_t *prev2 = (r >= 3) ? _rows[r - 2] : NULL; /* T(r-2, *) */

            for (idx_t k = 0; k < r; ++k) {
                seq_t val = 0;

                /* T(r-1, k) */
                if (k < r - 1) val += prev[k];

                /* T(r-1, k-1) */
                if (k > 0 && k - 1 < r - 1) val += prev[k - 1];

                /* T(r-2, k-1)  (subtracted) */
                if (prev2 && k > 0 && k - 1 < r - 2)
                    val -= prev2[k - 1];

                /* T(r-2, k-2)  (added) */
                if (prev2 && k > 1 && k - 2 < r - 2)
                    val += prev2[k - 2];

                row[k] = val;
            }
        }

        _rows[r] = row;
    }

    _rows_cnt = n;
}

/* Public function: compute a(n) ------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of defined range, return 0 */

    /* find row r such that sum_{i=1}^{r-1} i < n <= sum_{i=1}^{r} i */
    idx_t r = 1;
    idx_t cum = 1;                 /* sum of first row lengths */
    while (cum < n) {
        ++r;
        cum += r;
    }
    idx_t prev_cum = cum - r;
    idx_t k = n - prev_cum - 1;   /* 0‑based column index within row r */

    _ensure_rows(r);
    return _rows[r][k];
}
