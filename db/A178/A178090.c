/*
 * A178090  Coefficient array for orthogonal polynomials
 *          P(n,x)=x*P(n-1,x)-floor((n-1)/2)*P(n-2,x), P(0,x)=1,P(1,x)=x.
 * https://oeis.org/A178090
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 58, i16 n <= 81, i32 n <= 81, i64 n <= 81, i128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: P(0,x)=1, P(1,x)=x, P(n,x)=x*P(n-1,x)-⌊(n‑1)/2⌋*P(n‑2,x).
   a(n) is the flattened coefficient array (row by row). */
#include <stdlib.h>

static seq_t **coef_rows = NULL;
static idx_t rows_alloc = 0;
static idx_t max_row_computed = (idx_t)(-1);

/* Ensure space for at least 'needed' row pointers. */
static void ensure_rows_alloc(idx_t needed)
{
    if (needed <= rows_alloc) return;
    seq_t **tmp = (seq_t**)realloc(coef_rows, (size_t)needed * sizeof(seq_t*));
    if (!tmp) exit(EXIT_FAILURE);       /* allocation failure */
    for (idx_t i = rows_alloc; i < needed; ++i) tmp[i] = NULL;
    coef_rows = tmp;
    rows_alloc = needed;
}

/* Compute all rows up to and including row 'r'.  The results are stored
   in the global array 'coef_rows' and the global variable 'max_row_computed'
   is updated. */
static void compute_rows_up_to(idx_t r)
{
    /* Create row 0 if this is the first call. */
    if (max_row_computed == (idx_t)(-1)) {
        seq_t *row0 = (seq_t*)malloc(sizeof(seq_t));
        if (!row0) exit(EXIT_FAILURE);
        row0[0] = 1;
        ensure_rows_alloc(1);
        coef_rows[0] = row0;
        max_row_computed = 0;
    }

    if (max_row_computed >= r) return;   /* already done */

    ensure_rows_alloc(r + 1);

    for (idx_t i = max_row_computed + 1; i <= r; ++i) {
        seq_t *row = (seq_t*)malloc((size_t)(i + 1) * sizeof(seq_t));
        if (!row) exit(EXIT_FAILURE);

        if (i == 1) {               /* P(1,x)=x */
            row[0] = 0;
            row[1] = 1;
        } else {                    /* i >= 2 */
            seq_t mult = (seq_t)((i - 1) / 2);   /* floor((i‑1)/2) */
            for (idx_t k = 0; k <= i; ++k) {
                seq_t term_a = 0;
                if (k > 0) term_a = coef_rows[i - 1][k - 1];

                seq_t term_b = 0;
                if (k <= i - 2) term_b = coef_rows[i - 2][k];

                row[k] = term_a - term_b * mult;
            }
        }
        coef_rows[i] = row;
    }

    max_row_computed = r;
}

seq_t a(idx_t n)
{
    /* Find the row r such that n lies within that row. */
    idx_t r = 0;
    while ((r + 1U) * (r + 2U) / 2U <= n) ++r;

    idx_t offset = r * (r + 1U) / 2U;
    idx_t k = n - offset;

    compute_rows_up_to(r);
    return coef_rows[r][k];
}
