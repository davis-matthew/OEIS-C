/*
 * A209573  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209574; see the Formula section.
 * https://oeis.org/A209573
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

/*  A209573 – Triangle of coefficients of polynomials u(n,x) jointly generated
 *  with A209574.
 *
 *  Definition
 *  ----------
 *  u(1,x) = 1,            v(1,x) = 1
 *  u(n,x) = x*u(n-1,x) + v(n-1,x)
 *  v(n,x) = 2x*u(n-1,x) + (x+1)*v(n-1,x) + 1
 *
 *  Let r be the unique integer such that r(r+1)/2 ≥ n
 *  (i.e., the row index in the triangle whose first element has global
 *  index n₀ = 1 + r(r-1)/2).  Then
 *
 *      a(n) = coefficient of x^(k-1) in u(r,x)
 *            where k = n - r(r-1)/2
 *
 *  Initial values: a(1)=1, a(2)=1, a(3)=1, a(4)=2, a(5)=4,
 *  a(6)=1, a(7)=3, a(8)=9, a(9)=9, a(10)=1, …
 *
 *  The function below computes a(n) by memoising all previously computed
 *  rows of the u-polynomials, which guarantees O(n) time for
 *  successive calls.  The algorithm is fully generic: it works for any
 *  unsigned integer type seq_t which supports the required operators.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* forward declared helper: compute rows up to a given index */
static void compute_rows_up_to(idx_t r);

/* static storage for memoised rows */
static seq_t **u_rows      = NULL;   /* array of pointers to stored u rows */
static size_t u_rows_cap   = 0;      /* current capacity of u_rows array   */
static size_t rows_alloc   = 0;      /* number of rows actually stored     */

/* for recurrence */
static seq_t *prev_u = NULL; /* most recently computed u row */
static size_t prev_len = 0;  /* length of prev_u (row size) */

static seq_t *prev_v = NULL; /* most recently computed v row */
static size_t v_len   = 0;    /* length of prev_v */

/* ------------------------------------------------------------------------- */
/*  Find the row number (starting at 1) that contains the global index n.
 *  The row r satisfies r(r+1)/2 ≥ n, and r-1 is the first row whose
 *  cumulative total is < n.
 */
static idx_t row_of(idx_t n)
{
    idx_t lo = 1, hi = 1;
    /* double hi until the triangular number exceeds n */
    while ((unsigned __int128)hi * (hi + 1) / 2 < n)
        hi <<= 1;

    /* binary search in [lo, hi] */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* ------------------------------------------------------------------------- */
/*  Compute and memoise all rows of u and v up to row r (inclusive). */
static void compute_rows_up_to(idx_t r)
{
    while (rows_alloc < r) {
        size_t next_len = rows_alloc + 1; /* length of the new row */

        /* allocate for new u and v rows */
        seq_t *new_u = (seq_t *)malloc(next_len * sizeof(seq_t));
        seq_t *new_v = (seq_t *)malloc(next_len * sizeof(seq_t));
        if (!new_u || !new_v) abort();

        /* ----- base case for the first row ----- */
        if (rows_alloc == 0) {
            new_u[0] = (seq_t)1;
            new_v[0] = (seq_t)1;
        } else {
            /* ----- recurrence for u ----- */
            for (size_t i = 0; i < next_len; ++i) {
                seq_t from_prev = (i > 0) ? prev_u[i - 1] : (seq_t)0;
                seq_t from_v    = (i < prev_len) ? prev_v[i] : (seq_t)0;
                new_u[i] = from_prev + from_v;
            }

            /* ----- recurrence for v ----- */
            new_v[0] = prev_v[0] + (seq_t)1;
            for (size_t i = 1; i < next_len; ++i) {
                seq_t term1 = (seq_t)2 * prev_u[i - 1];
                seq_t term2 = (i < prev_len) ? prev_v[i] : (seq_t)0;
                seq_t term3 = prev_v[i - 1];
                new_v[i] = term1 + term2 + term3;
            }
        }

        /* store the computed u row persistently */
        if (rows_alloc == u_rows_cap) {
            size_t new_cap = (u_rows_cap == 0) ? 4 : u_rows_cap * 2;
            seq_t **tmp = (seq_t **)realloc(u_rows, new_cap * sizeof(seq_t *));
            if (!tmp) abort();
            u_rows = tmp;
            u_rows_cap = new_cap;
        }
        u_rows[rows_alloc] = new_u;

        /* update bookkeeping */
        rows_alloc = next_len;
        prev_u = new_u;
        prev_len = next_len;

        /* free old v row (if any) and make new_v the current one */
        if (prev_v) free(prev_v);
        prev_v = new_v;
        v_len = next_len;
    }
}

/* ------------------------------------------------------------------------- */
/*  Public function: returns the n-th term of A209573. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, but harmless */

    /* determine the row r containing n */
    idx_t r = row_of(n);

    /* make sure all rows up to r are computed */
    compute_rows_up_to(r);

    /* compute the offset within the row (0‑based) */
    idx_t prev_sum = (r - 1) * r / 2;      /* triangular number T_(r-1) */
    idx_t offset   = n - prev_sum - 1;     /* index within row r */

    /* return the requested coefficient */
    return u_rows[r - 1][offset];
}
