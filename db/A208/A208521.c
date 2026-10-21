/*
 * A208521  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A208520; see the Formula section.
 * https://oeis.org/A208521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the coefficient of x^k in the polynomial v(m,x), where m
 *            is the unique integer with T(m‑1) < n ≤ T(m), T(m)=m(m+1)/2. The
 *            polynomials u and v are defined by (OEIS A208520/521) u(1,x) = 1
 *            , v(1,x) = 1 u(m,x) = u(m-1,x) + x·v(m-1,x) v(m,x) = 2x·u(m-1,x)
 *            + v(m-1,x) + 1 The sequence is obtain...
 * Range:     u8 n <= 40, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache of polynomial coefficient arrays */
static idx_t   max_row = 0;          /* number of rows already computed (≥0) */
static seq_t **u_rows  = NULL;      /* u coefficients: u_rows[0] is row 1 */
static seq_t **v_rows  = NULL;      /* v coefficients: v_rows[0] is row 1 */
static size_t  rows_cap = 0;        /* capacity of the row pointer arrays */

/* enlarge the row pointer arrays when needed */
static void grow_rows_cap(void)
{
    size_t new_cap = rows_cap ? rows_cap * 2 : 8;
    seq_t **new_u = (seq_t**)realloc(u_rows, new_cap * sizeof(seq_t*));
    seq_t **new_v = (seq_t**)realloc(v_rows, new_cap * sizeof(seq_t*));
    if (!new_u || !new_v) abort();          /* out of memory */
    u_rows = new_u;
    v_rows = new_v;
    rows_cap = new_cap;
}

/* compute all rows up to ‘target’ (inclusive) */
static void ensure_rows(idx_t target)
{
    /* target is a 1‑based row number */
    while (max_row < target)
    {
        idx_t n = max_row + 1;          /* next row to compute, 1‑based */

        /* allocate coefficient arrays for this row */
        seq_t *new_u = (seq_t*)malloc(n * sizeof(seq_t));
        seq_t *new_v = (seq_t*)malloc(n * sizeof(seq_t));
        if (!new_u || !new_v) abort();

        if (n == 1)                    /* base case */
        {
            new_u[0] = 1;
            new_v[0] = 1;
        }
        else
        {
            seq_t *prev_u = u_rows[max_row - 1];   /* row m-1 (0‑based) */
            seq_t *prev_v = v_rows[max_row - 1];

            /* compute u(n,x) = u(n‑1,x) + x·v(n‑1,x) */
            new_u[0] = prev_u[0];
            for (idx_t k = 1; k < n; ++k)
            {
                seq_t cu = (k < n - 1) ? prev_u[k] : 0;   /* coefficient of x^k in u(n‑1,x) */
                seq_t cv = prev_v[k - 1];                 /* coefficient of x^(k-1) in v(n‑1,x) */
                new_u[k] = cu + cv;
            }

            /* compute v(n,x) = 2x·u(n‑1,x) + v(n‑1,x) + 1 */
            new_v[0] = prev_v[0] + 1;
            for (idx_t k = 1; k < n; ++k)
            {
                seq_t cv = (k < n - 1) ? prev_v[k] : 0;   /* coefficient of x^k in v(n‑1,x) */
                seq_t cu = prev_u[k - 1];                 /* coefficient of x^(k-1) in u(n‑1,x) */
                new_v[k] = cv + (seq_t)2 * cu;
            }
        }

        /* store the newly computed row */
        if (max_row >= rows_cap) grow_rows_cap();
        u_rows[max_row] = new_u;
        v_rows[max_row] = new_v;
        ++max_row;                            /* rows 1..max_row are now ready */
    }
}

/* public sequence routine */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* a(1) is the first term */

    /* find the row r such that T(r‑1) < n ≤ T(r),  with T(r)=r(r+1)/2 */
    idx_t r = 0;
    while (r * (r + 1) / 2 < n) ++r;          /* r will be at most ≈ sqrt(2n) */

    /* ensure rows 1..r have been computed */
    ensure_rows(r);

    /* compute the offset within the row (0‑based) */
    idx_t start_of_row = (r - 1) * r / 2 + 1; /* first index belonging to row r */
    idx_t offset = n - start_of_row;          /* 0‑based offset within the row */

    return v_rows[r - 1][offset];             /* rows stored 0‑based in the array */
}
