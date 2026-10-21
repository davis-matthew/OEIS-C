/*
 * A208343  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A208342; see the Formula section.
 * https://oeis.org/A208343
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row, col) where T(n,k) satisfies T(n,k) = T(n-1,k) +
 *            T(n-1,k-1) + T(n-2,k-2) - T(n-2,k-1), with initial values:
 *            T(0,0)=1; T(1,0)=0; T(1,1)=2; and T(n,k)=0 if k>n or k<0. The
 *            sequence a(n) is the flattened triangle read row‑by‑row
 * Range:     u8 n <= 73, u16 n <= 76, u32 n <= 76, u64 n <= 76, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Persistent storage for the triangle: triangle[r][k] = T(r,k). */
static seq_t **triangle = NULL;
static idx_t   max_rows  = 0;     /* next row to allocate is max_rows */

/* Build rows up to `row_needed` (inclusive). */
static void ensure_rows(idx_t row_needed)
{
    if (row_needed < max_rows)
        return; /* already computed */

    /* Grow the pointer array. */
    triangle = (seq_t**)realloc(triangle,
                                (row_needed + 1) * sizeof(seq_t*));

    for (idx_t r = max_rows; r <= row_needed; ++r) {
        /* Allocate row r of length r+1. */
        triangle[r] = (seq_t*)malloc((r + 1) * sizeof(seq_t));

        if (r == 0) {                    /* T(0,0)=1 */
            triangle[0][0] = (seq_t)1;
        } else if (r == 1) {             /* T(1,0)=0, T(1,1)=2 */
            triangle[1][0] = (seq_t)0;
            triangle[1][1] = (seq_t)2;
        } else {                         /* general recurrence */
            for (idx_t k = 0; k <= r; ++k) {
                unsigned long long a = (k <= r-1) ? (unsigned long long)triangle[r-1][k] : 0ULL;
                unsigned long long b = (k >= 1)     ? (unsigned long long)triangle[r-1][k-1] : 0ULL;
                unsigned long long c = (k >= 2)     ? (unsigned long long)triangle[r-2][k-2] : 0ULL;
                unsigned long long d = (k >= 1 && k-1 <= r-2) ? (unsigned long long)triangle[r-2][k-1] : 0ULL;

                unsigned long long sum = a + b + c;
                sum -= d; /* guaranteed non‑negative in the integer domain */

                triangle[r][k] = (seq_t)sum;
            }
        }
    }
    max_rows = row_needed + 1;
}

/* Compute a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;         /* undefined below the offset */

    /* Find the row r such that cumulative count up to row r-1 < n ≤ cumulative up to row r. */
    idx_t cum = 0;  /* number of entries before current row */
    idx_t r   = 0;
    while (cum + (r + 1) < n) {
        cum += r + 1;
        ++r;
    }

    ensure_rows(r);

    /* Column index within the row (0‑based). */
    idx_t col = n - cum - 1;

    return triangle[r][col];
}
