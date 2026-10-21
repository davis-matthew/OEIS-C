/*
 * A210804  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210803; see the Formula section.
 * https://oeis.org/A210804
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle of coefficients of polynomials v(n,x) (OEIS A210804).
   The function returns the flattened triangle value a(n) where
   a(1)=1, a(2)=2, a(3)=2, a(4)=5, ... as defined by the recurrence:
     T(1,0)=1,
     T(2,0)=T(2,1)=2,
     T(n,k) = 4*T(n-1,k) + T(n-1,k-1)
              - 3*T(n-2,k) - 2*T(n-2,k-1)
              + T(n-2,k-2)   for n>=3,
   with T(n,k)=0 for k<0 or k>=n.  The flattened index n (1‑based)
   is converted to row r and column k by solving r(r+1)/2 >= n.
   The implementation memoizes rows up to the maximum required. */

#include <stdlib.h>
#include <math.h>

/* Integer square root for 64‑bit unsigned integers.  Result is floor(sqrt(x)). */
static inline idx_t
isqrt_u64(idx_t x)
{
    idx_t r = (idx_t)sqrt((double)x);
    while ((r + 1) * (r + 1) <= x) ++r;
    while (r * r > x) --r;
    return r;
}

/* Cached rows of the triangle.  rows[1] holds the first row, etc. */
static seq_t **rows   = NULL; /* array of pointers to rows */
static size_t rows_cap = 0;   /* current capacity of rows[] */
static idx_t   max_row    = 0;/* largest row already computed */

/* Ensure that rows 1 … needed are allocated and filled. */
static void
ensure_rows(idx_t needed)
{
    if (needed <= max_row)
        return;                   /* nothing to do */

    /* Grow the rows array if necessary. */
    if (rows_cap < (size_t)(needed + 1)) {
        seq_t **tmp = (seq_t **)realloc(rows,
                                        (needed + 1) * sizeof(seq_t *));
        rows = tmp;
        rows_cap = needed + 1;
    }

    /* Allocate and compute new rows. */
    for (idx_t r = max_row + 1; r <= needed; ++r) {
        rows[r] = (seq_t *)malloc(sizeof(seq_t) * r);   /* r elements: 0 … r-1 */
        if (r == 1) {
            rows[1][0] = (seq_t)1;                     /* T(1,0)=1   */
        } else if (r == 2) {
            rows[2][0] = (seq_t)2;                     /* T(2,0)=2   */
            rows[2][1] = (seq_t)2;                     /* T(2,1)=2   */
        } else {
            /* Compute T(r,k) for k = 0 … r-1. */
            for (idx_t k = 0; k < r; ++k) {
                seq_t a = 0, b = 0, c = 0, d = 0, e = 0;

                /* a = T(r-1,k)      (defined only if k < r-1) */
                if (k < r - 1)
                    a = rows[r - 1][k];

                /* b = T(r-1,k-1)    (defined only if k-1 >= 0) */
                if (k >= 1)
                    b = rows[r - 1][k - 1];

                /* c = T(r-2,k)      (defined only if k < r-2) */
                if (k < r - 2)
                    c = rows[r - 2][k];

                /* d = T(r-2,k-1)    (defined only if k-1 >= 0) */
                if (k >= 1 && k - 1 < r - 2)
                    d = rows[r - 2][k - 1];

                /* e = T(r-2,k-2)    (defined only if k-2 >= 0) */
                if (k >= 2)
                    e = rows[r - 2][k - 2];

                /* recurrence: 4a + b - 3c - 2d + e */
                seq_t pos = (seq_t)4 * a + b + e;
                seq_t neg = (seq_t)3 * c + (seq_t)2 * d;
                rows[r][k] = pos - neg;  /* values are non‑negative */
            }
        }
    }
    max_row = needed;
}

/* Main function: returns the nth term of A210804 (1‑based). */
seq_t
a(idx_t n)
{
    if (n == 0)            /* not defined for offset 1 */
        return (seq_t)0;

    /* Compute the row r such that r*(r+1)/2 >= n. */
    idx_t s = isqrt_u64(1 + 8 * n);          /* floor(sqrt(1+8n)) */
    idx_t r = (s - 1) / 2;                    /* initial guess (floor) */

    /* Adjust to the first r with cumulative sum >= n. */
    while ((unsigned __int128)r * (r + 1) / 2 < n) ++r;

    /* Column index within row (0‑based).  Elements before this row = (r‑1)*r/2. */
    idx_t before = (idx_t)((unsigned __int128)(r - 1) * r / 2);
    idx_t k = n - before - 1;

    /* Ensure that row r is computed. */
    ensure_rows(r);

    return rows[r][k];
}
