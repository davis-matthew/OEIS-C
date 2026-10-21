/*
 * A230002  Array of coefficients of numerator polynomials of the rational
 *          function p(n, x - 1/x), where p(n,x) is the Fibonacci polynomial
 *          defined by p(1,x) = 1, p(2,x) = x, p(n,x) = x*p(n-1,x) + p(n-2,x).
 * https://oeis.org/A230002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 79, i16 n <= 79, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A230002 – coefficients of the numerator of p(n,x-1/x)
 *  a(n) is the n‑th term in the concatenation of the coefficient lists
 *  of the polynomials R_n(x) = x^{n-1} p(n,x-1/x) for n = 1,2,... .
 *
 *  The polynomials satisfy the recurrence
 *
 *      R_1(x) = 1,
 *      R_2(x) = x^2 – 1,
 *      R_n(x) = (x^2 – 1) R_{n-1}(x) + x^2 R_{n-2}(x)    (n ≥ 3)
 *
 *  The returned value a(idx) is the coefficient of x^k where
 *  k = idx – (r-1)^2 and r = ⌊√idx⌋ + 1 is the row containing the
 *  coefficient.
 */
#include <stddef.h>
#include <stdlib.h>

/* integer square root: floor(sqrt(n)) */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    idx_t low = 0;
    idx_t high = (idx_t)1ull << 32;    /* high^2 > UINT64_MAX */
    while (low + 1 < high) {
        idx_t mid = (low + high) / 2;
        if (mid <= n / mid)        /* mid*mid ≤ n without overflow */
            low = mid;
        else
            high = mid;
    }
    return low;
}

/* global storage for the computed rows of R_n(x) */
static seq_t **polys = NULL;          /* array of pointers to coefficient arrays */
static idx_t   max_row_computed = 0;  /* number of rows stored (n=1..max_row_computed) */
static size_t  polys_cap = 0;

/* ensure we have stored rows up to required_row (n = required_row) */
static void ensure_rows(idx_t required_row)
{
    if (required_row <= max_row_computed)
        return;

    /* enlarge polys array if necessary */
    if (required_row > polys_cap) {
        size_t new_cap = polys_cap ? polys_cap * 2 : required_row;
        seq_t **tmp = (seq_t **)realloc(polys, new_cap * sizeof(seq_t *));
        if (!tmp) abort();          /* out of memory */
        polys     = tmp;
        polys_cap = new_cap;
    }

    /* build missing rows */
    for (idx_t n = max_row_computed + 1; n <= required_row; ++n) {
        size_t len = 2 * (size_t)n - 1;          /* 2n-1 coefficients */
        seq_t *new_poly = (seq_t *)calloc(len, sizeof(seq_t));
        if (!new_poly) abort();

        if (n == 1) {                    /* R_1(x) = 1 */
            new_poly[0] = 1;
        } else if (n == 2) {              /* R_2(x) = x^2 – 1 */
            new_poly[0] = -1;
            new_poly[2] = 1;
        } else {
            /* retrieve previous two polynomials */
            seq_t *prev    = polys[n - 2];   /* R_{n-1} */
            seq_t *prev2   = polys[n - 3];   /* R_{n-2} */
            size_t len_prev  = 2 * (size_t)(n - 1) - 1;   /* 2(n-1)-1 */
            size_t len_prev2 = 2 * (size_t)(n - 2) - 1;   /* 2(n-2)-1 */

            /* (x^2 – 1) * R_{n-1}  => shift prev by +2 and subtract prev */
            for (size_t i = 0; i < len_prev; ++i) {
                new_poly[i + 2] += prev[i];      /* x^2 * prev */
                new_poly[i]     -= prev[i];      /* -1 * prev */
            }
            /* + x^2 * R_{n-2} */
            for (size_t i = 0; i < len_prev2; ++i) {
                new_poly[i + 2] += prev2[i];
            }
        }

        polys[n - 1] = new_poly;
    }

    max_row_computed = required_row;
}

seq_t a(idx_t n)
{
    /* determine the row (n = row index starting at 1) that contains
     * the coefficient with overall index n (0‑based).  Row r satisfies
     * r^2 > n ≥ (r-1)^2.
     */
    idx_t row = floor_sqrt_u64(n) + 1;       /* 1‑based row number */
    idx_t offset = n - (row - 1) * (row - 1);/* 0‑based offset within the row */

    /* compute rows up to 'row' if not yet done */
    ensure_rows(row);

    seq_t *poly = polys[row - 1];            /* pointer to the row's coefficient array */
    return poly[offset];
}
