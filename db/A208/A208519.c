/*
 * A208519  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A208518; see the Formula section.
 * https://oeis.org/A208519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) is the (n‑th) element read row‑wise from the triangle of
 * coefficients of the polynomials v(n,x) defined by
 *
 *   u(1,x) = 1,                           v(1,x) = 1
 *   u(n,x) = u(n-1,x) + x·v(n-1,x)
 *   v(n,x) = x·u(n-1,x) + (x+1)·v(n-1,x) + 1
 *
 * The triangle is produced row by row; row r contains r coefficients of v(r,x).
 * The sequence a(n) is the usual flattened enumeration of the triangle:
 *   a(1)=1,
 *   a(2)=2, a(3)=2,
 *   a(4)=3, a(5)=5, a(6)=3, …
 *
 * The implementation keeps the successive rows of u and v in memory,
 * appends each new row of v to a growing flat array, and serves `a(n)`
 * by indexing that array.
 */

#include <stddef.h>
#include <stdlib.h>
#include <math.h>

/* ------------------------------------------------------------------------- */
/*  Static state used to memoise computed rows and values                      */
static size_t  seq_cap  = 0;     /* allocated capacity of seq_vals          */
static size_t  seq_len  = 0;     /* number of terms actually stored        */
static seq_t *seq_vals = NULL;   /* flattened sequence of v coefficients   */
static seq_t *u_prev   = NULL;   /* coefficients of u for previous row     */
static seq_t *v_prev   = NULL;   /* coefficients of v for previous row     */
static idx_t   prev_len = 0;     /* length of the previous row             */
static idx_t   max_row  = 0;     /* highest row that has been computed    */

/* ------------------------------------------------------------------------- */
/* Ensure that seq_vals has capacity at least `needed` elements.           */
static void ensure_res_cap(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap * 2 : 64;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(seq_vals, new_cap * sizeof(seq_t));
    seq_vals = tmp;
    seq_cap  = new_cap;
}

/* ------------------------------------------------------------------------- */
/* Initialise row 1 (the base case) if it has not yet been done.             */
static void init_base(void)
{
    if (max_row != 0)
        return;                     /* already initialised */

    /* row 1 has length 1 */
    seq_t *row1 = (seq_t *)malloc(sizeof(seq_t));
    row1[0] = (seq_t)1;

    /* store the coefficient of v(1,x) in the flat sequence */
    ensure_res_cap(seq_len + 1);
    seq_vals[seq_len++] = row1[0];

    /* initialise the previous‑row arrays */
    u_prev   = (seq_t *)malloc(sizeof(seq_t));
    v_prev   = (seq_t *)malloc(sizeof(seq_t));
    u_prev[0] = (seq_t)1;
    v_prev[0] = (seq_t)1;
    prev_len  = 1;
    max_row   = 1;
}

/* ------------------------------------------------------------------------- */
/* Compute and cache all rows up to `need`, inclusive.                       */
static void ensure_rows_up_to(idx_t need)
{
    if (need <= max_row)
        return;

    init_base();   /* guarantee that row 1 is ready */

    for (idx_t n = max_row + 1; n <= need; ++n) {
        idx_t curr_len = n;                         /* length of new row */
        seq_t *u_curr = (seq_t *)malloc(curr_len * sizeof(seq_t));
        seq_t *v_curr = (seq_t *)malloc(curr_len * sizeof(seq_t));

        /* ----- compute u_curr ------------------------------------------- */
        for (idx_t k = 0; k < curr_len; ++k) {
            seq_t val = (k < prev_len ? u_prev[k] : 0);
            if (k > 0)
                val += v_prev[k - 1];
            u_curr[k] = val;
        }

        /* ----- compute v_curr ------------------------------------------- */
        for (idx_t k = 0; k < curr_len; ++k) {
            seq_t val = 0;
            if (k > 0) {
                val += u_prev[k - 1];   /* x·u_prev[k-1] */
                val += v_prev[k - 1];   /* x·v_prev[k-1] */
            }
            if (k < prev_len)
                val += v_prev[k];      /* v_prev[k]   */
            if (k == 0)
                val += (seq_t)1;       /* constant term */
            v_curr[k] = val;
        }

        /* ----- append the new row to the flattened sequence ---------- */
        ensure_res_cap(seq_len + curr_len);
        for (idx_t k = 0; k < curr_len; ++k)
            seq_vals[seq_len++] = v_curr[k];

        /* ----- recycle memory for the next iteration ------------------ */
        free(u_prev);
        free(v_prev);
        u_prev   = u_curr;
        v_prev   = v_curr;
        prev_len = curr_len;
        max_row  = n;
    }
}

/* ------------------------------------------------------------------------- */
/* Locate the row index `r` such that the first `r` rows contain at least
 * `n` terms.  For n ≤ 1, r = 1.                                                  */
static idx_t row_for_index(idx_t n)
{
    double d  = sqrt((double)(8.0 * n - 7.0));
    idx_t r   = (idx_t)((d + 1.0) / 2.0);  /* floor of the formula */
    /* guard against off-by-one due to floating‑point inaccuracy */
    if (r * (r + 1) / 2 < n)
        ++r;
    return r;
}

/* ------------------------------------------------------------------------- */
/* Public function required by the judge.                                       */
seq_t a(idx_t n)
{
    if (n == 0)               /* the sequence is 1‑based; helper unused */
        return (seq_t)0;

    idx_t needed_row = row_for_index(n);
    ensure_rows_up_to(needed_row);
    return seq_vals[n - 1];
}
