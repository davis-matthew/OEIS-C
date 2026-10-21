/*
 * A209159  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209158; see the Formula section.
 * https://oeis.org/A209159
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   u(1,x) = 1, v(1,x) = 1 u(n,x) = u(n-1,x) + (x+1)*v(n-1,x) v(n,x)
 *            = 2*x*u(n-1,x) + x*v(n-1,x) + 1 The triangle of coefficients of
 *            v(n,x), read row by row, gives the OEIS sequence A209159. Thus
 *            a(1)=1, a(2)=1, a(3)=3, a(4)=1, …
 * Range:     u8 n <= 25, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
#include <new>   /* for placement new in the allocator */
#endif

/* helper to allocate an array of seq_t with proper construction
   (C++ only) */
static seq_t *alloc_seq_array(size_t n)
{
#ifdef __cplusplus
    seq_t *p = (seq_t*)malloc(n * sizeof(seq_t));
    for (size_t i = 0; i < n; ++i)
        new(&p[i]) seq_t();             /* default-construct each element   */
    return p;
#else
    return (seq_t*)malloc(n * sizeof(seq_t));
#endif
}

/* global state for memoisation */
static seq_t **v_rows = NULL;   /* v_rows[r] points to the coefficients of v(r,x) */
static seq_t  *u_prev = NULL;   /* coefficients of u(r-1,x)   */
static seq_t  *v_prev = NULL;   /* coefficients of v(r-1,x)   */
static size_t  max_row = 0;     /* highest row produced        */

/* compute the smallest r with r*(r+1)/2 >= n   (n>=1) */
static size_t row_from_index(idx_t n)
{
    size_t lo = 1, hi = 1;
    /* grow hi until it covers n */
    while ((hi * (hi + 1ULL) / 2ULL) < n) hi <<= 1U;
    /* binary search */
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1U;
        if ((mid * (mid + 1ULL) / 2ULL) >= n)
            hi = mid;
        else
            lo = mid + 1U;
    }
    return lo;
}

/* ensure that rows 1..r_needed are computed and stored in v_rows */
static void ensure_rows(size_t r_needed)
{
    if (r_needed <= max_row) return;

    /* allocate or enlarge the array of row pointers */
    if (v_rows == NULL) {
        v_rows = (seq_t**)malloc((r_needed + 1) * sizeof(seq_t*));
    } else {
        v_rows = (seq_t**)realloc(v_rows, (r_needed + 1) * sizeof(seq_t*));
    }

    /* initialise first row if this is the very first call */
    if (max_row == 0) {
        v_rows[1] = alloc_seq_array(1);   /* v(1,x) has one coefficient = 1 */
        v_rows[1][0] = 1;
        u_prev = alloc_seq_array(1);      /* u(1,x) = 1 */
        u_prev[0] = 1;
        v_prev = v_rows[1];               /* v_prev points to row 1 */
        max_row = 1;
    }

    /* generate successive rows up to r_needed */
    for (size_t row = max_row + 1; row <= r_needed; ++row) {
        size_t prev_len = row - 1;          /* coefficients of previous row */
        size_t new_len  = row;             /* coefficients of current row   */

        seq_t *u_new = alloc_seq_array(new_len);
        seq_t *v_new = alloc_seq_array(new_len);

        /* compute u_new[0] = u_prev[0] + v_prev[0] + (1)*v_prev[-1]? */
        /* For k = 0: (x+1)*v_prev[0] with x term absent; so just u_prev[0] + v_prev[0] */
        u_new[0] = u_prev[0] + v_prev[0];

        /* compute u_new[1 .. prev_len-1] */
        for (size_t k = 1; k < prev_len; ++k) {
            /* u(n,x) coefficient for term x^k: u_prev[k] + (k+1)*v_prev[k] */
            /* but (x+1)*v_prev = v_prev[k]*x^k + v_prev[k-1]*x^k   => add v_prev[k-1] */
            u_new[k] = u_prev[k] + v_prev[k] + v_prev[k - 1];
        }
        /* last coefficient: u_prev[prev_len-1] + v_prev[prev_len-1] (since k = prev_len-1) */
        u_new[prev_len] = v_prev[prev_len - 1];   /* u_prev has no element prev_len */

        /* compute v_new coefficients */
        /* v(n,x) = 2*x*u_prev + x*v_prev + 1  => coefficient for x^0 is 1        */
        v_new[0] = 1;
        for (size_t k = 1; k < new_len; ++k) {
            /* coefficient for x^k: 2*(k)*u_prev[k-1] + (k)*v_prev[k-1]   */
            /* but since indices start at 0, the multiplier is just 2*u_prev[k-1] + v_prev[k-1] */
            v_new[k] = (seq_t)(2 * u_prev[k - 1] + v_prev[k - 1]);
        }

        /* free previous u array (v_prev remains for next iteration) */
        free(u_prev);

        /* store the new v row */
        v_rows[row] = v_new;

        /* prepare for next iteration */
        u_prev = u_new;
        v_prev = v_new;
    }

    max_row = r_needed;
}

/* public function required by the exercise */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* undefined, return 0 */
    size_t row = row_from_index(n);           /* find the row containing n */
    size_t prev_terms = (row - 1) * row / 2;  /* number of terms before this row */
    size_t pos = (size_t)n - prev_terms - 1;  /* zero‑based column in the row   */

    ensure_rows(row);                          /* make sure the row exists */

    return v_rows[row][pos];
}
