/*
 * A209687  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A208339; see the Formula section.
 * https://oeis.org/A209687
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = T(n-1,k)+2*T(n-1,k-1)-T(n-2,k-1)+T(n-2,k-2) * with
 *            T(1,0)=1, T(2,0)=0, T(2,1)=2 and T(n,k)=0 for k<0 or k≥n. * The
 *            OEIS sequence a(n) is the flattened triangle read row by row
 * Range:     u8 n <= 33, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cached rows of the triangle.  rows[i] points to an array of i
 * seq_t values representing T(i,0..i-1).
 */
static seq_t **rows = NULL;         /* pointer to array of row pointers   */
static idx_t  rows_cap = 0;         /* highest n for which rows[n] is filled */

/* Ensure that rows 1..r are computed and cached. */
static void ensure_rows(idx_t r)
{
    if (r <= rows_cap)
        return;

    /* Extend the rows pointer array. */
    size_t new_cap = (size_t)(r + 1);
    seq_t **tmp = (seq_t **)realloc(rows, new_cap * sizeof(seq_t *));
    if (!tmp) {
        /* Allocation failed – aborting so that the program does not
         * continue with an incomplete cache.  Since this project
         * builds in a test harness, this is acceptable. */
        abort();
    }
    rows = tmp;

    /* Compute each missing row. */
    for (idx_t i = rows_cap + 1; i <= r; ++i) {
        if (i == 1) {
            rows[1] = (seq_t *)malloc(sizeof(seq_t) * 1);
            rows[1][0] = (seq_t)1;
        } else if (i == 2) {
            rows[2] = (seq_t *)malloc(sizeof(seq_t) * 2);
            rows[2][0] = (seq_t)0;
            rows[2][1] = (seq_t)2;
        } else {
            seq_t *prev    = rows[i - 1];
            seq_t *preprev = rows[i - 2];
            seq_t *cur     = (seq_t *)malloc(sizeof(seq_t) * i);
            seq_t two = (seq_t)2;

            for (idx_t j = 0; j < i; ++j) {
                seq_t a = (j < i - 1) ? prev[j] : (seq_t)0;
                seq_t b = (j > 0) ? prev[j - 1] : (seq_t)0;
                seq_t c = (j > 0  && j - 1 < i - 2) ? preprev[j - 1] : (seq_t)0;
                seq_t d = (j > 1) ? preprev[j - 2] : (seq_t)0;

                /* cur[j] = a + 2*b - c + d  (all operations are defined
                 * for unsigned/ arbitrary-precision seq_t and are
                 * guaranteed to be non‑negative). */
                seq_t temp = a + two * b + d;
                cur[j] = temp - c;
            }
            rows[i] = cur;
        }
    }
    rows_cap = r;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 >= n.  Since n is 1‑based, the
     * rows are numbered 1,2,3,... and each row r has r elements. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* Position k within the row (0‑based). */
    idx_t prev_t = ((r - 1) * r) / 2; /* triangular number of r-1 */
    idx_t k      = n - prev_t - 1;

    /* Compute rows up to r as needed. */
    ensure_rows(r);

    return rows[r][k];
}
