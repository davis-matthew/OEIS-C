/*
 * A157013  Riordan's general Eulerian recursion: T(n, k) = (k+2)*T(n-1, k) +
 *          (n-k-1) * T(n-1, k-1) with T(n,1) = 1, T(n,n) = (-1)^(n-1).
 * https://oeis.org/A157013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,k) where the triangle T is defined by T(1,1) = 1;
 *            T(n,1) = 1; T(n,n) = (-1)^(n-1); T(n,k) = (k+2)*T(n-1,k) +
 *            (n-k-1)*T(n-1,k-1) (1 < k < n) The sequence lists the entries of
 *            this triangle in row‑major order
 * Range:     i8 n <= 16, i16 n <= 38, i32 n <= 94, i64 n <= 237,
 *            i128 n <= 607, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

#ifdef __cplusplus
static std::vector< std::vector<seq_t> > _rows;
static idx_t _row_max = 0;
#else
static seq_t **_rows = NULL;
static idx_t _row_max = 0;
#endif

seq_t a(idx_t n)
{
    if (n < 1) return 0;       /* offset is 1; behaviour below offset is undefined */

    /* Find the row r that contains the n‑th element. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    /* Ensure that rows 1..r have been generated. */
#ifdef __cplusplus
    if (_rows.empty()) _rows.push_back(std::vector<seq_t>());  /* dummy 0‑index */
    while (_row_max < r)
    {
        idx_t newrow = _row_max + 1;
        std::vector<seq_t> row(newrow + 1);          /* index 0 unused */
        row[1] = 1;                                 /* T(newrow,1) = 1 */

        if (newrow > 1)
        {
            const std::vector<seq_t> &prev = _rows[newrow - 1];
            for (idx_t k = 2; k <= newrow - 1; ++k)
            {
                row[k] = (seq_t)(k + 2) * prev[k]
                       + (seq_t)(newrow - k - 1) * prev[k - 1];
            }
        }
        row[newrow] = ((newrow & 1) ? (seq_t)1 : (seq_t)-1);  /* (-1)^(newrow-1) */

        _rows.push_back(std::move(row));
        ++_row_max;
    }
#else
    while (_row_max < r)
    {
        idx_t newrow = _row_max + 1;
        seq_t **tmp_rows = (seq_t**)realloc(_rows,
                                        (newrow + 1) * sizeof(seq_t*));
        if (!tmp_rows) return 0;             /* allocation failure */
        _rows = tmp_rows;

        seq_t *row = (seq_t*)calloc(newrow + 1, sizeof(seq_t));  /* index 0 unused */
        if (!row) return 0;                 /* allocation failure */

        row[1] = 1;                         /* T(newrow,1) = 1 */

        if (newrow > 1)
        {
            seq_t *prev = _rows[newrow - 1];
            for (idx_t k = 2; k <= newrow - 1; ++k)
            {
                row[k] = (seq_t)(k + 2) * prev[k]
                       + (seq_t)(newrow - k - 1) * prev[k - 1];
            }
        }
        row[newrow] = ((newrow & 1) ? (seq_t)1 : (seq_t)-1);      /* (-1)^(newrow-1) */

        _rows[newrow] = row;
        ++_row_max;
    }
#endif

    /* Compute the column index k within the row r. */
    idx_t tri_prev = ((r - 1) * r) / 2;          /* elements before row r */
    idx_t k = n - tri_prev;                     /* 1‑based column index */

    /* Return the cached value. */
#ifdef __cplusplus
    return _rows[r][k];
#else
    return _rows[r][k];
#endif
}
