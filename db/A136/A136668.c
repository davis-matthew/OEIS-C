/*
 * A136668  Triangle read by rows: coefficients of a Bessel polynomial
 *          recursion: P(x, n) = 2*(n-1)*P(x, n - 1)/x - n*P(x, n - 2) with
 *          substitution x -> 1/y.
 * https://oeis.org/A136668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 18, i16 n <= 33, i32 n <= 73, i64 n <= 183,
 *            i128 n <= 325, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows: coefficients of a Bessel polynomial recursion.
   P_0(y)=1, P_1(y)=y, 
   recurrence (after substitution x=1/y): 
   P_n(y) = 2·(n-1)·y·P_{n-1}(y) – n·P_{n-2}(y)      (n≥2)
   a(n) is the (n‑1)th element in row‑major order (a(1)=1).         */
#include <stdlib.h>
#include <stddef.h>
#if defined(__cplusplus)
# include <new>
#endif

/* static storage for the triangle (row 0 first) */
static seq_t *triangle = NULL;
/* last row that has been computed (row indices start at 0) */
static idx_t rows_used = 0;
/* total number of allocated slots in 'triangle' */
static size_t allocated_terms = 0;

/* auxiliary: reallocate triangle array if needed */
static void
ensure_size(size_t new_terms)
{
#if defined(__cplusplus)
    seq_t *new_tri = new seq_t[new_terms];
    if (triangle) {
        for (size_t i = 0; i < allocated_terms; ++i)
            new_tri[i] = triangle[i];
        delete [] triangle;
    }
    triangle = new_tri;
#else
    if (triangle) {
        triangle = (seq_t *)realloc(triangle, new_terms * sizeof(seq_t));
    } else {
        triangle = (seq_t *)malloc(new_terms * sizeof(seq_t));
    }
#endif
    allocated_terms = new_terms;
}

/* ensure that triangle rows 0 … rmax are computed */
static void
ensure_rows(idx_t rmax)
{
    /* initial allocation and row 0 */
    if (!triangle) {
        /* total terms up to row rmax */
        size_t new_terms = (size_t)((__uint128_t)(rmax + 1) * (rmax + 2)) / 2;
        ensure_size(new_terms);
        /* row 0 */
        triangle[0] = 1;
        rows_used = 0;
    }

    /* reallocate if more space is required */
    size_t required_terms = (size_t)((__uint128_t)(rmax + 1) * (rmax + 2)) / 2;
    if (required_terms > allocated_terms)
        ensure_size(required_terms);

    /* compute missing rows */
    for (idx_t n = rows_used + 1; n <= rmax; ++n) {
        idx_t start = n * (n + 1) / 2;          /* index of first element in row n */

        if (n == 1) {
            /* row 1: coefficients of P_1(y)=y  -> 0, 1 */
            triangle[start + 0] = 0;
            triangle[start + 1] = 1;
        } else if (n >= 2) {
            idx_t start_prev1 = (n - 1) * n / 2;          /* row n-1 */
            idx_t start_prev2 = (n - 2) * (n - 1) / 2;    /* row n-2 */

            seq_t mul1 = (seq_t)(2 * (n - 1));           /* 2·(n-1)  */
            seq_t mul2 = (seq_t)n;                       /* n        */

            for (idx_t k = 0; k <= n; ++k) {
                seq_t term1 = 0;
                if (k >= 1)
                    term1 = mul1 * triangle[start_prev1 + (k - 1)];

                seq_t term2 = 0;
                if (k <= n - 2)
                    term2 = mul2 * triangle[start_prev2 + k];

                triangle[start + k] = term1 - term2;
            }
        }
        /* n==0 is already handled in the initialisation */
    }
    rows_used = rmax;
}

/* Return the nth term (1‑based).  Index below 1 returns 0. */
seq_t
a(idx_t n)
{
    if (n < 1)
        return 0;

    /* find the row r such that (r+1)(r+2)/2 ≥ n  */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 < n)
        ++r;

    ensure_rows(r);

    idx_t col_start = r * (r + 1) / 2;
    idx_t k = n - col_start - 1;          /* 0‑based column index */

    return triangle[col_start + k];
}
