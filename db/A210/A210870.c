/*
 * A210870  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210871; see the Formula section.
 * https://oeis.org/A210870
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th term of the flattened triangle of coefficients
 *            of u(n,x), defined by u(1,x)=1 , v(1,x)=1
 *            u(n,x)=u(n‑1,x)+x*v(n‑1,x)
 *            v(n,x)=(x+1)*u(n‑1,x)+(x‑1)*v(n‑1,x)+1 The sequence a(n) is the
 *            coefficient of x^(k‑1) in u(k,x) where k satisfies k*(k+1)/2 ≥ n
 * Range:     u8 n <= 61, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

/*  External integer types supplied by the build system:
    idx_t  – unsigned 64‑bit index type
    seq_t  – unsigned integer or arbitrary‑precision type        */
 
/* Static cache for rows of u and v.  The indices are 1‑based.  */
static size_t rows_computed = 0;   /* number of rows already computed   */
static size_t rows_capacity  = 0;  /* allocated capacity for row pointers   */
static seq_t **u_rows = NULL;      /* pointer array for rows of u         */
static seq_t **v_rows = NULL;      /* pointer array for rows of v         */

/* Ensure row pointer arrays have at least min_cap slots. */
static void grow_row_pointers(size_t min_cap)
{
    if (min_cap <= rows_capacity)
        return;
    size_t new_cap = rows_capacity ? rows_capacity << 1 : 8;
    while (new_cap < min_cap)
        new_cap <<= 1;
    seq_t **u_new = (seq_t **)realloc(u_rows, new_cap * sizeof(seq_t*));
    seq_t **v_new = (seq_t **)realloc(v_rows, new_cap * sizeof(seq_t*));
    if (!u_new || !v_new) abort();
    u_rows = u_new;
    v_rows = v_new;
    rows_capacity = new_cap;
}

/* Integer binary search: smallest r with r*(r+1)/2 ≥ n. */
static size_t row_of(idx_t n)
{
    size_t low = 1, high = 1;
    /* increase high until it is large enough */
    while (((__int128)high * (high + 1) / 2) < n)
        high <<= 1;
    while (low < high) {
        size_t mid = low + (high - low) / 2;
        idx_t val = (idx_t)((__int128)mid * (mid + 1) / 2);
        if (val >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Compute rows up to and including r, storing them in the cache. */
static void ensure_rows_upto(size_t r)
{
    if (r <= rows_computed)
        return;
    grow_row_pointers(r + 1);                /* we need indices 1…r */
    for (size_t i = rows_computed + 1; i <= r; ++i) {
        /* Allocate row i (length i). */
        seq_t *u = (seq_t *)calloc(i, sizeof(seq_t));
        seq_t *v = (seq_t *)calloc(i, sizeof(seq_t));
        if (!u || !v) abort();

        if (i == 1) {                 /* base case */
            u[0] = 1;
            v[0] = 1;
            u_rows[1] = u;
            v_rows[1] = v;
            continue;
        }

        seq_t *prev_u = u_rows[i - 1];
        seq_t *prev_v = v_rows[i - 1];

        u[0] = 1;                       /* constant term of u(i,x) */
        v[0] = prev_u[0] - prev_v[0] + 1;/* constant term of v(i,x) */

        for (size_t k = 1; k < i; ++k) {
            /* u[i][k] = u[i‑1][k] + v[i‑1][k‑1]   (k‑1 exists for k>=1) */
            seq_t u_prev_k  = (k < i - 1) ? prev_u[k] : 0;
            seq_t v_prev_km1 = prev_v[k - 1];
            u[k] = u_prev_k + v_prev_km1;

            /* v[i][k] = (x+1)u[i-1][k-1] + (x-1)v[i-1][k-1]
                       + (x+1)u[i-1][k]   + (x-1)v[i-1][k] + 1
               but collecting terms gives simpler recurrence:
               v[i][k] = u[i-1][k-1] + u[i-1][k] + v[i-1][k-1] - v[i-1][k] */
            seq_t u_prev_km1 = prev_u[k - 1];
            seq_t u_prev_kval = (k < i - 1) ? prev_u[k] : 0;
            seq_t v_prev_km1_val = prev_v[k - 1];
            seq_t v_prev_k_val   = (k < i - 1) ? prev_v[k] : 0;
            v[k] = u_prev_km1 + u_prev_kval + v_prev_km1_val - v_prev_k_val;
        }

        u_rows[i] = u;
        v_rows[i] = v;
    }
    rows_computed = r;
}

/* Required public function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* safety guard – undefined for n=0 */

    /* Find row r containing the nth flattened element */
    size_t r = row_of(n);

    /* Position inside row r (0‑based) */
    idx_t prev_sum = (idx_t)((__int128)(r - 1) * r / 2);
    size_t pos = (size_t)(n - prev_sum - 1);

    /* Compute necessary rows */
    ensure_rows_upto(r);

    /* Return the corresponding coefficient of u(r,x) */
    return u_rows[r][pos];
}
