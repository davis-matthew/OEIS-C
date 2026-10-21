/*
 * A208527  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A208526; see the Formula section.
 * https://oeis.org/A208527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A208527: Triangle of coefficients of polynomials v(n,x)
   defined by  u(n,x)=u(n-1,x)+x*v(n-1,x),
                v(n,x)=2x*u(n-1,x)+2x*v(n-1,x)+1,
                with u(1,x)=1, v(1,x)=1.
   The flattened sequence is obtained by reading the triangle
   row‑by‑row: a(1) = 1, a(2) = 1, a(3) = 4, …   */
#include <stdlib.h>

/* cache for the v‑polynomial coefficient rows */
static seq_t **v_rows          = NULL;  /* v_rows[r] -> pointer to array of length r */
static size_t v_rows_capacity  = 0;     /* allocated capacity for v_rows  */
static size_t max_computed     = 0;     /* maximum row already computed          */

/* previous row polynomials */
static seq_t *u_prev = NULL;
static seq_t *v_prev = NULL;
static size_t prev_size = 0;            /* size of previous row (= max_computed) */

/* --------------------------------------------------------------------- */
/* ensure that rows 1 … target are computed and stored in v_rows.        */
static void extend_to(idx_t target)
{
    if (target <= (idx_t)max_computed)
        return;

    /* ensure we can index v_rows[target] */
    if (v_rows_capacity < (size_t)target) {
        size_t new_cap = (size_t)target;
        v_rows = (seq_t**)realloc(v_rows, sizeof(seq_t*) * (new_cap + 1));
        v_rows_capacity = new_cap;
    }

    /* initialise first row */
    if (max_computed == 0) {
        u_prev = (seq_t*)calloc(1, sizeof(seq_t));
        v_prev = (seq_t*)calloc(1, sizeof(seq_t));
        u_prev[0] = (seq_t)1;
        v_prev[0] = (seq_t)1;
        v_rows[1] = v_prev;                 /* row 1 stored */
        max_computed = 1;
        prev_size = 1;
    }

    /* build rows max_computed+1 … target */
    for (size_t r = max_computed + 1; r <= (size_t)target; ++r) {
        size_t sz = r;                     /* number of coefficients in row r */
        seq_t *u_curr = (seq_t*)calloc(sz, sizeof(seq_t));
        seq_t *v_curr = (seq_t*)calloc(sz, sizeof(seq_t));

        /* v(r,x): constant term then coefficients of x^k, k=1…r-1 */
        v_curr[0] = (seq_t)1;              /* constant term 1 */
        for (size_t k = 1; k < sz; ++k) {
            seq_t sum = u_prev[k-1];
            sum += v_prev[k-1];
            v_curr[k] = sum << 1;          /* multiply by 2 */
        }

        /* u(r,x) */
        u_curr[0] = u_prev[0];
        for (size_t k = 1; k < sz; ++k) {
            seq_t val = (k < prev_size ? u_prev[k] : (seq_t)0);
            val += v_prev[k-1];
            u_curr[k] = val;
        }

        /* store v_curr in cache */
        v_rows[r] = v_curr;

        /* free previous u_prev (v_prev is still referenced by v_rows) */
        free(u_prev);

        /* advance to next row */
        u_prev = u_curr;
        v_prev = v_curr;
        prev_size = sz;
        max_computed = r;
    }
}

/* --------------------------------------------------------------------- */
/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)   /* sequence is defined for n ≥ 1 */
        return (seq_t)0;

    /* find the row r such that r(r+1)/2 ≥ n > (r-1)r/2 */
    idx_t target = n;
    idx_t low = 1;
    idx_t high = target;
    idx_t r = high;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri >= target) {
            r = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    extend_to(r);                       /* make sure row r is known */

    /* index within the row (0‑based) */
    idx_t prev_tri = (r - 1) * r / 2;    /* total terms before row r */
    idx_t k = target - prev_tri - 1;     /* 0‑based position in v_r */

    return v_rows[r][(size_t)k];
}
