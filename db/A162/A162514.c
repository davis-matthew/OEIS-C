/*
 * A162514  Triangle of coefficients of polynomials defined by the Binet form
 *          P(n,x) = U^n + L^n, where U = (x + d)/2, L = (x - d)/2, d = (4 +
 *          x^2)^(1/2). Decreasing powers of x.
 * https://oeis.org/A162514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(r,m) = T(r-1,m) + T(r-2,m-2) (r≥2, 0≤m≤r) with T(0,0)=2,
 *            T(1,0)=1, T(1,1)=0. The sequence a(n) is the “flattened” list of
 *            the triangle rows: a(n)=T(r,m) where r*(r+1)/2 ≤ n <
 *            (r+1)*(r+2)/2 and m = n − r*(r+1)/2. (Offset: 0. a(0)=2.)
 * Range:     u8 n <= 112, u16 n <= 364, u32 n <= 1300, u64 n <= 4802,
 *            u128 n <= 5150, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* Integer square root of a uint64_t, floor.  No overflow. */
static idx_t
int_sqrt_u64(idx_t val)
{
    idx_t lo = 0, hi = (val >> 1) + 1;
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid <= val / mid)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* Lazy storage of the triangle rows.  tri[r] points to an array of
   length r+1 holding T(r,0)…T(r,r).  */
static seq_t **tri = NULL;          /* array of rows */
static idx_t  tri_rows = 0;         /* number of rows allocated */

/* Ensure that rows 0…needed are allocated and filled. */
static void
ensure_rows(idx_t needed)
{
    if (tri_rows > needed)
        return;

    /* Allocate or grow the row pointer array. */
    idx_t new_rows = needed + 1;
    seq_t **tmp = (tri_rows == 0)
        ? (seq_t**)malloc(sizeof(seq_t*) * new_rows)
        : (seq_t**)realloc(tri, sizeof(seq_t*) * new_rows);
    if (!tmp)
        abort();                /* OOM – does not happen for n≤10000 */
    tri = tmp;

    /* Allocate the new rows. */
    for (idx_t r = tri_rows; r <= needed; ++r) {
        tri[r] = (seq_t*)calloc((size_t)(r + 1), sizeof(seq_t));
        if (!tri[r])
            abort();            /* OOM – does not happen for n≤10000 */
    }

    /* Compute the rows up to `needed`. */
    for (idx_t r = 0; r <= needed; ++r) {
        if (r == 0) {                   /* T(0,0)=2 */
            tri[0][0] = (seq_t)2;
        } else if (r == 1) {             /* T(1,0)=1, T(1,1)=0 */
            tri[1][0] = (seq_t)1;
            tri[1][1] = (seq_t)0;
        } else {                         /* General recurrence */
            for (idx_t m = 0; m <= r; ++m) {
                seq_t term1 = (m <= r - 1) ? tri[r - 1][m] : (seq_t)0;
                seq_t term2 = (m >= 2 && m - 2 <= r - 2)
                              ? tri[r - 2][m - 2] : (seq_t)0;
                tri[r][m] = term1 + term2;
            }
        }
    }
    tri_rows = needed + 1;
}

/* Main sequence function. */
seq_t
a(idx_t n)
{
    /* Find the row r that contains index n.  The starting index of
       row r is r*(r+1)/2.  Solve r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t s = int_sqrt_u64(8 * n + 1);
    idx_t r = (s - 1) / 2;
    idx_t row_start = r * (r + 1) / 2;
    idx_t m = n - row_start;

    ensure_rows(r);
    return tri[r][m];
}
