/*
 * A116077  Triangular matrix T, read by rows, such that the anticommutator of
 *          T and U shifts the columns of T up 1 row: {T,U}(n,k) = T(n+1,k),
 *          where U denotes the triangular matrix defined by U(n,k) =
 *          A000108(n-k) = Catalan(n-k) for n>=k and where T(n,n) = (n+1).
 * https://oeis.org/A116077
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (row, column) entry T(row,col) of a triangular
 *            matrix defined by T(r,r) = r+1 T(r,c) = T(r-1,c-1) + C(2*(r-c),
 *            (r-c)) for 1 ≤ c ≤ r-1 T(r,0) = ∑_{m=0}^{r-1} (
 *            T(r-1,m)*Catalan(m) + Catalan(r-1-m)*T(m,0) ) where C is the
 *            binomial coefficient and Catalan is the Catalan sequence. The
 *            seq...
 * Range:     u8 n <= 14, u16 n <= 44, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binom, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/catalan.h"
#include "../utils/binom.h"

static seq_t **rows      = NULL;   /* array of pointers to rows */
static size_t row_cap    = 0;       /* capacity of rows array          */
static size_t row_cnt    = 0;       /* number of rows currently stored */

static seq_t *first_col  = NULL;   /* first column values of each row */
static size_t first_cap  = 0;       /* capacity of first_col            */

/* Ensure that rows up to 'need' (inclusive) are computed and stored. */
static void ensure_rows(size_t need) {
    /* grow rows array if necessary */
    if (row_cap < need + 1) {
        size_t new_cap = (row_cap == 0) ? 1 : row_cap * 2;
        while (new_cap < need + 1) new_cap *= 2;
        seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
        if (!tmp) abort();
        rows = tmp;
        row_cap = new_cap;
    }

    /* grow first_col array if necessary */
    if (first_cap < need + 1) {
        size_t new_cap = (first_cap == 0) ? 1 : first_cap * 2;
        while (new_cap < need + 1) new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(first_col, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        first_col = tmp;
        first_cap = new_cap;
    }

    /* Compute missing rows */
    while (row_cnt <= need) {
        size_t r     = row_cnt;            /* current row number */
        size_t len   = r + 1;              /* number of elements in this row */
        seq_t *row   = (seq_t *)malloc(len * sizeof(seq_t));
        if (!row) abort();

        /* Set the last element: T(r,r) = r+1 */
        row[r] = (seq_t)(r + 1);

        if (r > 0) {
            /* Compute first column via anticommutator sum */
            seq_t sumS1 = 0, sumS2 = 0;
            seq_t *prev_row = rows[r - 1];
            for (size_t m = 0; m < r; ++m) {
                seq_t cat_m      = catalan((idx_t)m);                     /* Catalan(m)   */
                sumS1 += prev_row[m] * cat_m;                            /* T(r-1,m)*Cat(m) */

                seq_t cat_rm     = catalan((idx_t)((r - 1) - m));         /* Cat((r-1)-m)*/
                sumS2 += cat_rm * first_col[m];                         /* * T(m,0)     */
            }
            row[0] = sumS1 + sumS2;

            /* Compute the remaining columns */
            for (size_t c = 1; c < r; ++c) {
                size_t diff = r - c;                                     /* r-c > 0      */
                seq_t central = binom((idx_t)(2 * diff), (idx_t)diff);   /* C(2m,m)      */
                row[c] = prev_row[c - 1] + central;
            }
        } else {
            /* For row 0: T(0,0) = 1, already set by row[r] = 1 above */
            row[0] = (seq_t)1;
        }

        /* Store the row and its first element */
        rows[row_cnt]      = row;
        first_col[row_cnt] = row[0];
        ++row_cnt;
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    size_t r = 0;
    while ((size_t)r * (r + 1) / 2 <= n) ++r;
    --r;
    size_t start = r * (r + 1) / 2;
    size_t c     = (size_t)n - start;

    ensure_rows(r);

    return rows[r][c];
}
