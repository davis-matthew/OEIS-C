/*
 * A208331  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A208330; see the Formula section.
 * https://oeis.org/A208331
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   The triangle T(n,k) (n≥1, 0≤k<n) is defined by T(1,0) = 1,
 *            T(2,0) = 1, T(2,1) = 3, T(n,k) = 2*T(n‑1,k) + T(n‑1,k‑1) -
 *            T(n‑2,k) - T(n‑2,k‑1) + 2*T(n‑2,k‑2) (terms with illegal indices
 *            are taken as 0). The sequence a(n) is the row‑major flattening
 *            of this triangle: a(n) = T(r,k) where r is the uniq...
 * Range:     u8 n <= 25, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static seq_t **triangle = NULL;   /* triangle[1..] */
static size_t triangle_rows = 0;  /* number of rows currently allocated */

/* Ensure that rows up to 'r' are computed and stored.  */
static void
ensure_rows(size_t r)
{
    if (r <= triangle_rows)
        return;

    /* Resize the triangle array if necessary */
    if (triangle_rows == 0) {
        triangle = (seq_t **)malloc((r + 1) * sizeof(seq_t *));
    } else {
        triangle = (seq_t **)realloc(triangle, (r + 1) * sizeof(seq_t *));
    }

    /* Build rows from the first unbuilt up to r */
    for (size_t n = triangle_rows + 1; n <= r; ++n) {
        /* Allocate row */
        triangle[n] = (seq_t *)malloc(n * sizeof(seq_t));

        if (n == 1) {                 /* base case row 1 */
            triangle[n][0] = 1;
        } else if (n == 2) {           /* base case row 2 */
            triangle[n][0] = 1;
            triangle[n][1] = 3;
        } else {                       /* recurrence for n >= 3 */
            size_t len_n       = n;
            size_t len_prev    = n - 1;
            size_t len_prev2   = n - 2;
            seq_t *prev_row    = triangle[n - 1];
            seq_t *prev2_row   = triangle[n - 2];

            for (size_t k = 0; k < len_n; ++k) {
                seq_t val = 0;

                /* 2 * T(n‑1,k) */
                if (k < len_prev) {
                    val += prev_row[k];
                    val += prev_row[k];
                }

                /* + T(n‑1,k‑1) */
                if (k > 0 && (k - 1) < len_prev) {
                    val += prev_row[k - 1];
                }

                /* – T(n‑2,k) */
                if (k < len_prev2) {
                    val -= prev2_row[k];
                }

                /* – T(n‑2,k‑1) */
                if (k > 0 && (k - 1) < len_prev2) {
                    val -= prev2_row[k - 1];
                }

                /* + 2 * T(n‑2,k‑2) */
                if (k > 1 && (k - 2) < len_prev2) {
                    val += prev2_row[k - 2];
                    val += prev2_row[k - 2];
                }

                triangle[n][k] = val;
            }
        }
    }
    triangle_rows = r;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n<1, but return 0 for safety */

    /* Find the row 'r' such that r(r‑1)/2 < n ≤ r(r+1)/2 */
    size_t r = 1;
    size_t cumulative = 1;
    while (n > cumulative) {
        ++r;
        cumulative += r;
    }

    /* offset within the row (0‑based) */
    size_t prev_cumulative = cumulative - r;
    size_t k = n - prev_cumulative - 1;

    /* Generate required rows */
    ensure_rows(r);

    return triangle[r][k];
}
