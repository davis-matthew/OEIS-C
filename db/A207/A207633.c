/*
 * A207633  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A207634; see the Formula section.
 * https://oeis.org/A207633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is the n-th coefficient when the rows of the triangle
 * of polynomials u(n,x) are written successively from n = 1 upward
 * (coefficients listed from the constant term up).  
 * Recurrence:
 *   u(1,x) = 1            v(1,x) = 1
 *   u(n,x) = u(n-1,x) + v(n-1,x)
 *   v(n,x) = (x+1)·u(n-1,x) + 2x·v(n-1,x) + 1
 *
 * The triangle rows have lengths: length(1)=1, length(n)=n-1 for n≥2.
 */

#include <stddef.h>
#include <stdlib.h>

/* Helper to allocate a row of given length (initialized to 0). */
static seq_t *alloc_row(size_t len)
{
    return (seq_t *)calloc(len, sizeof(seq_t));
}

/* Cached data: arrays of pointers to coefficient arrays of u and v. */
static idx_t      max_row = 0;
static size_t     rows_cap = 0;
static seq_t    **u_rows = NULL;
static seq_t    **v_rows = NULL;

/* Ensure capacity for at least new_cap rows (indexing starts at 1). */
static void ensure_rows_cap(size_t new_cap)
{
    if (new_cap <= rows_cap)
        return;
    size_t new_cap_inc = (new_cap > rows_cap * 2) ? new_cap : rows_cap * 2;
    u_rows = (seq_t **)realloc(u_rows, (new_cap_inc + 1) * sizeof(seq_t *));
    v_rows = (seq_t **)realloc(v_rows, (new_cap_inc + 1) * sizeof(seq_t *));
    rows_cap = new_cap_inc;
}

/* Find the row r (≥1) such that the cumulative number of terms up to r
   is ≥ n.  Uses the triangular counts for this triangle:
   cum(r) = 1 + (r-1)r/2,   len_row(r) = 1 for r=1, else r-1. */
static idx_t find_row(idx_t n)
{
    idx_t r = 1;
    idx_t cum = 1;                 /* cumulative terms up to current r */
    while (cum < n) {
        ++r;
        cum += (r - 1);            /* add length of the new row */
    }
    return r;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n < 1; return 0 as a safe fallback */

    /* Determine row r and index k within that row. */
    idx_t r = find_row(n);
    /* length of row r */
    idx_t len_row = (r == 1) ? 1 : (r - 1);
    /* cumulative count up to row r-1 */
    idx_t prev_total = (r == 1) ? 0 :
                       (1 + ((r - 2) * (r - 1)) / 2);
    idx_t k = n - prev_total - 1;   /* 0‑based index in row r */

    /* Expand cached rows up to r if needed. */
    while (max_row < r) {
        ++max_row;
        const idx_t prev_row = max_row - 1;

        if (max_row == 1) {   /* base row */
            ensure_rows_cap(1);
            seq_t *u_cur = alloc_row(1);
            seq_t *v_cur = alloc_row(1);
            u_cur[0] = 1;
            v_cur[0] = 1;
            u_rows[1] = u_cur;
            v_rows[1] = v_cur;
            continue;
        }

        /* Lengths of previous and current rows. */
        size_t len_u_prev = (prev_row == 1) ? 1 : (size_t)(prev_row - 1);
        size_t len_v_prev = (prev_row == 1) ? 1 : (size_t)prev_row;
        size_t len_u_cur  = (max_row == 1) ? 1 : (size_t)(max_row - 1);
        size_t len_v_cur  = (max_row == 1) ? 1 : (size_t)max_row;

        ensure_rows_cap(max_row);

        seq_t *u_cur = alloc_row(len_u_cur);
        seq_t *v_cur = alloc_row(len_v_cur);
        u_rows[max_row] = u_cur;
        v_rows[max_row] = v_cur;

        seq_t *u_prev = u_rows[prev_row];
        seq_t *v_prev = v_rows[prev_row];

        /* Compute u_cur = u_prev + v_prev (coefficients up to len_u_cur). */
        for (size_t i = 0; i < len_u_cur; ++i) {
            seq_t a = (i < len_u_prev) ? u_prev[i] : 0;
            seq_t b = (i < len_v_prev) ? v_prev[i] : 0;
            u_cur[i] = a + b;
        }

        /* Compute v_cur = (x+1)*u_prev + 2x*v_prev + 1. */
        for (size_t i = 0; i < len_v_cur; ++i) {
            seq_t term1 = 0;
            if (i < len_u_prev)
                term1 += u_prev[i];
            if (i > 0 && (i - 1) < len_u_prev)
                term1 += u_prev[i - 1];
            seq_t term2 = 0;
            if (i > 0 && (i - 1) < len_v_prev)
                term2 += (seq_t)2 * v_prev[i - 1];
            seq_t const_term = (i == 0) ? (seq_t)1 : (seq_t)0;
            v_cur[i] = term1 + term2 + const_term;
        }
    }

    /* Retrieve the requested coefficient. */
    return u_rows[r][k];
}
