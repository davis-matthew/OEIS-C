/*
 * A188316  Riordan array (1/(1-x^2), x/((1-x)*(1-x^2))).
 * https://oeis.org/A188316
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A188316
   Riordan array (1/(1‑x²), x/((1‑x)*(1‑x²)))
   Sequence: flattened triangle T(n,k) defined by
      T(0,0)   = 1
      T(1,0)   = 0,  T(1,1) = 1
      T(2,0)   = 1,  T(2,1) = 1,  T(2,2) = 1
      T(n,k)   = T(n‑1,k) + T(n‑1,k‑1) + T(n‑2,k) – T(n‑3,k)
      for n ≥ 3, k ≤ n, with T(n,k)=0 when k<0 or k>n.
   The sequence a(n) is the rows of T written linearly:
      a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1), a(3)=T(2,0), … .
*/

#include <stdlib.h>
#include <math.h>

/* Static cache for rows of the triangle */
static seq_t **triangle = NULL;        /* element rows, triangle[r] points to row r array */
static size_t  triangle_cap = 0;       /* capacity of triangle array */
static idx_t   rows_allocated = 0;     /* rows already computed */

static void ensure_rows(idx_t req)
{
    /* Ensure triangle array has room for indices 0..req */
    if (triangle_cap == 0) {
        triangle_cap = 8;
        triangle = (seq_t **)malloc(triangle_cap * sizeof(seq_t *));
    }
    while (triangle_cap <= req) {
        triangle_cap <<= 1;
        triangle = (seq_t **)realloc(triangle, triangle_cap * sizeof(seq_t *));
    }

    /* Compute missing rows */
    while (rows_allocated <= req) {
        idx_t r = rows_allocated;
        seq_t *row = (seq_t *)malloc((size_t)(r + 1) * sizeof(seq_t));

        /* Base cases */
        if (r == 0) {
            row[0] = (seq_t)1;
        } else if (r == 1) {
            row[0] = (seq_t)0;
            row[1] = (seq_t)1;
        } else if (r == 2) {
            row[0] = (seq_t)1;
            row[1] = (seq_t)1;
            row[2] = (seq_t)1;
        } else { /* r >= 3 */
            for (idx_t k = 0; k <= r; ++k) {
                seq_t val = (seq_t)0;
                if (k <= r-1)          val += triangle[r-1][k];           /* T(r-1,k)   */
                if (k > 0 && k <= r)   val += triangle[r-1][k-1];         /* T(r-1,k-1) */
                if (k <= r-2)          val += triangle[r-2][k];           /* T(r-2,k)   */
                if (k <= r-3)          val -= triangle[r-3][k];           /* -T(r-3,k)  */
                row[k] = val;
            }
        }

        triangle[r] = row;
        ++rows_allocated;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Determine row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    long double d = sqrtl((long double)8 * (long double)n + 1.0L);
    idx_t r = (idx_t)((d - 1.0L) / 2.0L);

    /* Correct possible rounding errors */
    idx_t tri = r * (r + 1) / 2;
    if (n < tri) {
        while (n < tri) {
            --r;
            tri = r * (r + 1) / 2;
        }
    } else {
        while ((r + 1) * (r + 2) / 2 <= n) {
            ++r;
        }
        tri = r * (r + 1) / 2;
    }
    idx_t k = n - tri;

    /* Ensure the required rows are computed */
    ensure_rows(r);

    return triangle[r][k];
}
