/*
 * A210756  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210755; see the Formula section.
 * https://oeis.org/A210756
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (n‑th) entry in the triangle of coefficients of
 *            v(n,x) where v(n,x) is defined by u(1,x)=1 , v(1,x)=1
 *            u(n,x)=(x+1)*u(n-1,x)+2x*v(n-1,x)+1
 *            v(n,x)=(x+1)*u(n-1,x)+(x+1)*v(n-1,x)+1 The triangle is read row
 *            by row (constant term first)
 * Range:     u8 n <= 17, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* -------------------------------------------------------------------------- */
/*  Static storage for the computed rows of the polynomials u and v. */
/* -------------------------------------------------------------------------- */

static seq_t **u_rows = NULL; /* u_rows[r] → array of r coefficients of u(r,x) */
static seq_t **v_rows = NULL; /* v_rows[r] → array of r coefficients of v(r,x) */
static idx_t  rows_used   = 0; /* number of rows currently computed (highest r) */
static size_t rows_cap    = 0; /* current capacity of the row arrays */

/* Ensure storage can hold at least 'need' rows (indices 0 … need).
   We ask for `need+1` because we use 1‑based indexing for rows. */
static void ensure_rows_cap(size_t need)
{
    if (rows_cap >= need)
        return;
    size_t new_cap = rows_cap ? rows_cap : 1;
    while (new_cap < need)
        new_cap <<= 1;
    u_rows = (seq_t **)realloc(u_rows, new_cap * sizeof *u_rows);
    v_rows = (seq_t **)realloc(v_rows, new_cap * sizeof *v_rows);
    rows_cap = new_cap;
}

/* Compute all rows up to at least 'desired' and store them.
   The initial row (n=1) is created if it does not exist. */
static void ensure_rows_to(idx_t desired)
{
    if (rows_used >= desired)
        return;

    /* Create the first row if this is the very first call. */
    if (rows_used == 0) {
        ensure_rows_cap(2);          /* indices 0 and 1 are needed */
        u_rows[1] = (seq_t *)malloc(sizeof *u_rows[1]); /* len = 1 */
        v_rows[1] = (seq_t *)malloc(sizeof *v_rows[1]);
        u_rows[1][0] = (seq_t)1;
        v_rows[1][0] = (seq_t)1;
        rows_used = 1;
    }

    ensure_rows_cap(desired + 1);    /* make room for the new rows */

    for (idx_t cur = rows_used + 1; cur <= desired; ++cur) {
        size_t len_prev = cur - 1;   /* previous row length */
        size_t len_curr = cur;       /* current row length */

        u_rows[cur] = (seq_t *)malloc(len_curr * sizeof *u_rows[cur]);
        v_rows[cur] = (seq_t *)malloc(len_curr * sizeof *v_rows[cur]);

        /* k = 0 */
        u_rows[cur][0] = u_rows[cur-1][0] + (seq_t)1;
        v_rows[cur][0] = u_rows[cur-1][0] + v_rows[cur-1][0] + (seq_t)1;

        /* k ≥ 1 */
        for (size_t k = 1; k < len_curr; ++k) {
            seq_t u_prev_k      = (k < len_prev) ? u_rows[cur-1][k] : (seq_t)0;
            seq_t u_prev_km1    = u_rows[cur-1][k-1];
            seq_t v_prev_km1    = v_rows[cur-1][k-1];
            u_rows[cur][k] = u_prev_k + u_prev_km1 + ((seq_t)2) * v_prev_km1;
        }

        for (size_t k = 1; k < len_curr; ++k) {
            seq_t u_prev_k      = (k < len_prev) ? u_rows[cur-1][k] : (seq_t)0;
            seq_t u_prev_km1    = u_rows[cur-1][k-1];
            seq_t v_prev_k      = (k < len_prev) ? v_rows[cur-1][k] : (seq_t)0;
            seq_t v_prev_km1    = v_rows[cur-1][k-1];
            v_rows[cur][k] = u_prev_k + u_prev_km1 + v_prev_k + v_prev_km1;
        }
    }
    rows_used = desired;
}

/* -------------------------------------------------------------------------- */
/*  Public function required by the problem statement.                           */
/* -------------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined, return 0 */

    /* Find the row r such that T(r-1) < n <= T(r)  (T(r)=r(r+1)/2). */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;

    /* Ensure computations up to row r are available. */
    ensure_rows_to(r);

    /* Position of the coefficient within the row (0‑based). */
    idx_t prev_T = (r - 1) * r / 2;
    size_t pos   = (size_t)(n - prev_T - 1);

    return v_rows[r][pos];
}
