/*
 * A172171  (1, 9) Pascal Triangle read by horizontal rows. Same as A093644,
 *          but mirrored and without the additional row/column (1, 9, 9, 9, 9,
 *          ...).
 * https://oeis.org/A172171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 143, u32 n <= 543, u64 n <= 1275,
 *            u128 n <= 1275, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes a(n) of OEIS A172171.
 * Triangle T(n,k) satisfies:
 *   T(n,k) = T(n-1,k) + 2*T(n-1,k-1) - T(n-2,k-1) - T(n-2,k-2),
 *   T(n,1) = 1,
 *   T(2,2) = 10,
 *   T(n,k) = 0 for k<1 or k>n.
 * a(n) is obtained by concatenating the rows of T, reading each row
 * left‑to‑right, starting with row 1.  The routine keeps a memoised
 * triangular table and expands it as required.
 */

#include <stdlib.h>

/*--- static data for memoisation ---------------------------------------*/
static seq_t **rows = NULL;          /* pointer array, 1‑based indexing */
static idx_t  max_precomp = 0;       /* largest n for which rows[n] is filled */
static size_t rows_cap = 0;          /* allocated capacity of rows[] */

/*--- helper to expand the triangular table up to a given row ---------*/
static void ensure_rows(idx_t upto)
{
    if (max_precomp >= upto) return;

    /* enlarge the rows pointer array if necessary */
    if ((size_t)upto + 2 > rows_cap) {
        size_t new_cap = (size_t)upto + 10;          /* add some slack */
        seq_t **tmp = (seq_t **)realloc(rows,
                                        sizeof(seq_t *) * (new_cap + 1));
        if (!tmp) abort();                            /* allocation failed */
        rows = tmp;
        rows_cap = new_cap;
    }

    /* compute rows from (max_precomp+1) up to 'upto' */
    for (idx_t n = max_precomp + 1; n <= upto; ++n) {
        /* allocate an array of size n+1 (ignore index 0) */
        seq_t *row = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!row) abort();
        rows[(size_t)n] = row;

        if (n == 1) {
            row[1] = 1;
        } else if (n == 2) {
            row[1] = 1;
            row[2] = 10;
        } else {
            row[1] = 1;               /* T(n,1)=1 */
            for (idx_t k = 2; k <= n; ++k) {
                seq_t a0 = (k <= n-1) ? rows[(size_t)(n-1)][(size_t)k] : 0;
                seq_t a1 = rows[(size_t)(n-1)][(size_t)(k-1)];     /* k-1≤n-1 */
                seq_t b0 = (k-1 <= n-2) ? rows[(size_t)(n-2)][(size_t)(k-1)] : 0;
                seq_t b1 = (k-2 <= n-2) ? rows[(size_t)(n-2)][(size_t)(k-2)] : 0;
                row[(size_t)k] = a0 + 2*a1 - b0 - b1;
            }
        }
    }
    max_precomp = upto;
}

/*--- public function ---------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* offset starts at 1 */

    /* determine the row r such that sum_{i=1}^{r-1} i < n <= sum_{i=1}^{r} i */
    idx_t r = 1;
    idx_t cum = 1;                        /* sum of lengths up to current row */
    while (cum < n) {
        ++r;
        cum += r;
    }

    /* column within that row (1‑based) */
    idx_t start = (r - 1) * r / 2;         /* elements before row r */
    idx_t col   = n - start;               /* 1 ≤ col ≤ r */

    ensure_rows(r);
    return rows[(size_t)r][(size_t)col];
}
