/*
 * A120058  Coefficients for obtaining A120057 from Bell numbers.
 * https://oeis.org/A120058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the n-th entry of the lower triangular array T(n,k)
 *            defined by the recurrence T(n,k) = 2*T(n-1,k) - 2*T(n-1,k-1) +
 *            2*T(n-2,k-1) - T(n-2,k), with T(n,1) = n (for n≥1). a(n) is
 *            obtained by flattening the rows of T row‑wise (row 1 first, then
 *            row 2, …). The first few rows are 1; 2,‑1; 3,‑4,...
 * Range:     i8 n <= 24, i16 n <= 55, i32 n <= 55, i64 n <= 55, i128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t **triangle = NULL;   /* 1‑based array of rows */
static idx_t maxRow = 0;

/* Ensure that rows 1 … upTo are computed and stored in `triangle`.  */
static void ensure_rows(idx_t upTo)
{
    if (triangle == NULL) {
        triangle = (seq_t **)malloc((upTo + 2) * sizeof(seq_t *));
    } else {
        triangle = (seq_t **)realloc(triangle,
                                    (upTo + 2) * sizeof(seq_t *));
    }

    for (idx_t r = maxRow + 1; r <= upTo; ++r) {
        triangle[r] = (seq_t *)malloc((r + 2) * sizeof(seq_t));  /* 1‑based */

        if (r == 1) {
            triangle[1][1] = (seq_t)1;
        } else if (r == 2) {
            triangle[2][1] = (seq_t)2;
            triangle[2][2] = (seq_t)-1;
        } else {
            triangle[r][1] = (seq_t)r;                /* T(r,1) = r */
            for (idx_t k = 2; k <= r; ++k) {
                seq_t a = (k <= r - 1) ? triangle[r - 1][k] : 0;
                seq_t b = (k - 1 >= 1 && k - 1 <= r - 1) ?
                          triangle[r - 1][k - 1] : 0;
                seq_t c = (k - 1 <= r - 2) ? triangle[r - 2][k - 1] : 0;
                seq_t d = (k <= r - 2) ? triangle[r - 2][k] : 0;
                triangle[r][k] = 2 * a - 2 * b + 2 * c - d;
            }
        }
    }
    maxRow = upTo;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined, return 0 */

    /* Find the row r such that (r-1)*r/2 < n ≤ r*(r+1)/2. */
    idx_t r = 1;
    idx_t cumulative = 1;                     /* sum of rows up to r */
    while (cumulative < n) {
        ++r;
        cumulative += r;
    }
    idx_t col = n - (cumulative - r);         /* 1‑based column index */

    ensure_rows(r);                           /* compute rows up to r */
    return triangle[r][col];
}
