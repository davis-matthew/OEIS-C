/*
 * A209420  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209419; see the Formula section.
 * https://oeis.org/A209420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 82, u32 n <= 261, u64 n <= 922,
 *            u128 n <= 1225, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the flattened triangle of the coefficients of
 * the polynomials v(n,x), where the triangle T(n,k) satisfies
 *
 *     T(n,k) = T(n-1,k) + 2*T(n-1,k-1) + T(n-2,k) - T(n-2,k-2),
 *     T(1,0) = 1,
 *     T(2,0) = T(2,1) = 2,
 *     T(n,k) = 0 for k < 0 or k >= n.
 *
 * The sequence a(n) is obtained by reading the rows of T from left
 * to right, top to bottom: a(1)=T(1,0), a(2)=T(2,0), a(3)=T(2,1),
 * a(4)=T(3,0), etc.
 *
 * The implementation keeps the triangle rows in a static cache,
 * computed on demand.  The function works for any seq_t/idx_t
 * types provided by the build environment.
 */

#include <stdlib.h>
#include <stddef.h>

/* Cache of triangle rows: rows[1] contains row 1, rows[2] contains row 2, etc. */
static seq_t **rows = NULL;
static idx_t  rows_cap = 0;   /* allocated capacity for rows array           */
static idx_t  max_row = 0;    /* highest row computed so far                */

/* Ensure that rows[1..r] are computed and cached.  If r <= max_row,
 * no action is performed.  Otherwise the cache is enlarged and new
 * rows are generated using the recurrence. */
static void ensure_rows(idx_t r)
{
    if (r <= max_row)
        return;

    /* Expand the rows array if necessary.  C++ requires an explicit cast. */
    if (rows_cap < r + 1) {
        size_t new_cap = (size_t)(r + 1);
        seq_t **tmp = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
        if (!tmp) exit(1);
        rows = tmp;
        rows_cap = new_cap;
    }

    /* Compute missing rows. */
    for (idx_t i = max_row + 1; i <= r; ++i) {
        /* Allocate storage for this row. */
        seq_t *row = (seq_t*)calloc((size_t)i, sizeof(seq_t));
        if (!row) exit(1);
        rows[i] = row;

        if (i == 1) {          /* base case T(1,0) = 1           */
            row[0] = (seq_t)1;
        } else if (i == 2) {   /* base case T(2,0)=T(2,1)=2      */
            row[0] = (seq_t)2;
            row[1] = (seq_t)2;
        } else {               /* recurrence for i >= 3          */
            seq_t *prev1 = rows[i-1];  /* row i-1   (length i-1)       */
            seq_t *prev2 = rows[i-2];  /* row i-2   (length i-2)       */
            for (idx_t k = 0; k < i; ++k) {
                seq_t val = 0;
                if (k < i-1)               /* T(i-1,k)   */
                    val += prev1[k];
                if (k > 0)                  /* 2*T(i-1,k-1) */
                    val += (seq_t)2 * prev1[k-1];
                if (k < i-2)               /* T(i-2,k)   */
                    val += prev2[k];
                if (k >= 2)                 /* -T(i-2,k-2) */
                    val -= prev2[k-2];
                row[k] = val;
            }
        }
    }
    max_row = r;
}

/* Return the nth term of A209420 (1‑based index). */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    /* Find the row that contains the nth term.  The cumulative count
     * of terms up to row r is r*(r+1)/2. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n) ++r;

    ensure_rows(r);

    /* Compute column index: 0‑based within the row. */
    idx_t col = n - ((r * (r - 1)) / 2 + (idx_t)1);
    return rows[r][col];
}
