/*
 * A180049  Coefficient triangle of the numerators of the (n-th convergents
 *          to) the continued fraction 1/(w+2/(w+3/(w+4/... .
 * https://oeis.org/A180049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 67, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) – coefficient read from the triangle of the numerators
 * of the continued fraction 1/(w+2/(w+3/(w+4/...)).
 * The polynomials b(n) are defined by
 *   b(0)=1,    b(1)=w,
 *   b(n)=w*b(n-1)+(n+1)*b(n-2)   (n≥2).
 * The triangle lists the coefficients of b(n) (degree n) in
 * row‑major order.  a(1)=1, a(2)=0, a(3)=1, a(4)=3, … as given
 * in the OEIS entry.
 */

#include <stdlib.h>

static seq_t **poly_rows = NULL;   /* poly_rows[d] → array of coefficients of b(d) */
static idx_t   max_degree = 0;     /* highest degree computed so far */
static size_t  poly_cap   = 0;     /* current capacity of poly_rows */

/* Ensure that all rows up to and including 'deg' are computed. */
static void ensure_polys_upto(idx_t deg)
{
    /* On first call initialise rows 0 and 1. */
    if (poly_rows == NULL) {
        poly_cap   = 2;   /* for degrees 0 and 1 */
        poly_rows  = (seq_t **)malloc(poly_cap * sizeof(seq_t *));
        /* b(0) = 1 */
        seq_t *b0 = (seq_t *)malloc(sizeof(seq_t));
        b0[0] = (seq_t)1;
        poly_rows[0] = b0;
        /* b(1) = w  →  0 + 1·w  */
        seq_t *b1 = (seq_t *)malloc(2 * sizeof(seq_t));
        b1[0] = (seq_t)0;
        b1[1] = (seq_t)1;
        poly_rows[1] = b1;
        max_degree = 1;
    }

    if (deg <= max_degree)
        return;

    /* Grow the array of row pointers if necessary. */
    if (poly_cap <= (size_t)deg) {
        size_t new_cap = poly_cap > 0 ? poly_cap : 1;
        while (new_cap <= (size_t)deg)
            new_cap <<= 1;
        poly_rows = (seq_t **)realloc(poly_rows, new_cap * sizeof(seq_t *));
        poly_cap   = new_cap;
    }

    /* Compute each new row up to 'deg'. */
    for (idx_t d = max_degree + 1; d <= deg; ++d) {
        size_t len = (size_t)d + 1;                   /* number of coefficients */
        seq_t *p   = (seq_t *)malloc(len * sizeof(seq_t));

        seq_t *prev1 = poly_rows[d - 1];
        seq_t *prev2 = (d >= 2) ? poly_rows[d - 2] : NULL;

        for (idx_t j = 0; j <= d; ++j) {
            seq_t term1 = (j > 0) ? prev1[j - 1] : (seq_t)0;
            seq_t term2 = (seq_t)0;
            if (d >= 2 && j <= d - 2)
                term2 = (seq_t)(d + 1) * prev2[j];   /* (n+1)*b(n-2) */
            p[j] = term1 + term2;
        }
        poly_rows[d] = p;
    }
    max_degree = deg;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;              /* invalid index, return 0 as a safe default */

    idx_t t = n - 1;                  /* overall zero‑based index */

    /* Find the row r (starting at 1) that contains this index. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 <= t)
        ++r;

    idx_t deg = r - 1;                 /* the polynomial degree of this row */
    idx_t c   = t - (r - 1) * r / 2;    /* column within that row (zero‑based) */

    ensure_polys_upto(deg);

    seq_t *row = poly_rows[deg];
    return row[c];
}
