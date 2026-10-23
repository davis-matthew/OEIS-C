/*
 * A121585  Triangle read by rows: T(n,k) is the number of deco polyominoes of
 *          height n and having k 1-cell columns starting at level 0
 *          (0<=k<=n). A deco polyomino is a directed column-convex polyomino
 *          in which the height, measured along the diagonal, is attained only
 *          in the last column.
 * https://oeis.org/A121585
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   The sequence a(n) is the flattened triangle of trianglatrix
 *            T(n,k) where T(0,0)=1, for n≥1: P(n,t) = (n‑1)! + (t + n – 2) ·
 *            P(n‑1,t), and P(n,t)=∑_{k=0}^{n}T(n,k)t^k. The array is read by
 *            rows: (0,0) , (1,0),(1,1) , (2,0),(2,1),(2,2) , …
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation details:
   * Pre‑compute rows of the triangle up to the required row.
   * Store coefficients in a static 2‑D array.
   * Use the recurrence to fill new rows.
   * For n≥2 :   T(n,0) = (n‑1)! + (n‑2)·T(n‑1,0)
                 T(n,k) = (n‑2)·T(n‑1,k) + T(n‑1,k‑1)  for 1≤k<n
                 T(n,n) = T(n‑1,n‑1)
   * For n==1:   T(1,0)=0 , T(1,1)=1
   * Factoring the factorials in a running variable to avoid
     recomputation.  The implementation is safe for unsigned seq_t
     arithmetic; overflows are undefined for values that do not
     fit in the target type, but those values are not checked.  This
     satisfies the sequence requirements.  */

#include <stddef.h>

#define MAX_ROWS 200      /* sufficient for n up to 10000 (row≈140) */

static seq_t triangle[MAX_ROWS][MAX_ROWS + 1];
static size_t row_len[MAX_ROWS];
static size_t current_rows = 0;   /* number of rows already computed */
static seq_t fact = 0;            /* (r-1)! for the next row r to be computed */
static int initialized = 0;

/* compute all triangle rows up to and including row_needed */
static void
ensure_row(size_t row_needed)
{
    if (!initialized)
    {
        /* initialise the first row n = 0 */
        triangle[0][0] = 1;
        row_len[0] = 1;
        current_rows = 1;          /* we have stored row 0 */
        fact = 1;                  /* 0! = 1 */
        initialized = 1;
    }

    while (current_rows <= row_needed)
    {
        size_t n = current_rows;   /* we will compute row n (0‑based) */
        seq_t *prev = triangle[n - 1];
        seq_t *cur  = triangle[n];

        if (n == 1)                /* special handling for row 1 */
        {
            cur[0] = 0;
            cur[1] = 1;
        }
        else
        {
            /* fact currently holds (n-1)! */
            cur[0] = fact + (seq_t)(n - 2) * prev[0];
            for (size_t k = 1; k < n; ++k)
            {
                cur[k] = (seq_t)(n - 2) * prev[k] + prev[k - 1];
            }
            cur[n] = prev[n - 1];
        }

        row_len[n] = n + 1;
        ++current_rows;
        fact *= (seq_t)n;   /* prepare fact for next row (n!) */
    }
}

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    size_t r = 0;
    while (r * (r + 1) / 2 <= (size_t)n)
    {
        ++r;
    }
    --r;                     /* now r satisfies the bound */

    /* Make sure we have computed that row */
    if (r >= current_rows)
        ensure_row(r);

    /* column index within the row */
    size_t offset = r * (r + 1) / 2;
    size_t k = (size_t)n - offset;

    return triangle[r][k];
}
