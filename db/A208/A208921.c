/*
 * A208921  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A208922; see the Formula section.
 * https://oeis.org/A208921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) comes from the triangle of coefficients of the polynomials
 *            * u(n,x) defined by * u(1,x) = 1, v(1,x) = 1 * u(n,x) = u(n-1,x)
 *            + 2x·v(n-1,x) * v(n,x) = (x+1)·u(n-1,x) + v(n-1,x) + 1 * The
 *            sequence a(n) lists these coefficients row by row (row r has r
 *            terms), * starting with offset 1. For an ind...
 * Range:     u8 n <= 24, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Forward declaration of integer square‑root helper. */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t low = 0, high = n;
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        if (mid > 0 && mid > n / mid) {
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }
    return high;
}

/* Global state for memoisation.  The arrays hold the coefficients of the
 * current row of u and v.  tmp_u/tmp_v are used to compute the next row. */
static seq_t *u      = NULL;
static seq_t *v      = NULL;
static seq_t *tmp_u  = NULL;
static seq_t *tmp_v  = NULL;
static idx_t   cur_n = 0;    /* number of rows already computed */
static size_t  cap   = 0;    /* current allocation capacity */

/* Initialise the static arrays on the first call. */
static void init_arrays(void)
{
    if (u != NULL) return;          /* already initialised */
    cap = 1;
    u     = (seq_t *)malloc(cap * sizeof(seq_t));
    v     = (seq_t *)malloc(cap * sizeof(seq_t));
    tmp_u = (seq_t *)malloc(cap * sizeof(seq_t));
    tmp_v = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!u || !v || !tmp_u || !tmp_v) {
        /* Allocation failed – leave the arrays NULL; the caller will
         * detect that a(…) cannot be computed. */
        return;
    }
    u[0] = v[0] = 1;       /* u(1,x) = 1,  v(1,x) = 1 */
    cur_n = 1;             /* we have computed row 1 */
}

/* Grow the allocation if the requested size exceeds current capacity. */
static void ensure_capacity(size_t new_cap)
{
    if (new_cap <= cap) return;
    seq_t *tmp;
    tmp = (seq_t *)realloc(u,   new_cap * sizeof(seq_t));
    if (!tmp) return;
    u = tmp;
    tmp = (seq_t *)realloc(v,   new_cap * sizeof(seq_t));
    if (!tmp) return;
    v = tmp;
    tmp = (seq_t *)realloc(tmp_u, new_cap * sizeof(seq_t));
    if (!tmp) return;
    tmp_u = tmp;
    tmp = (seq_t *)realloc(tmp_v, new_cap * sizeof(seq_t));
    if (!tmp) return;
    tmp_v = tmp;
    cap = new_cap;
}

/* The required OEIS sequence. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset is 1; below that behaviour not needed */

    init_arrays();
    if (!u || !v) return 0;       /* memory not allocated */

    /* Determine the row r and position k corresponding to index n. */
    idx_t t   = n - 1;                          /* zero‑based index */
    idx_t disc = 8 * t + 1;                     /* 8t+1 fits in 64‑bit */
    idx_t root = isqrt_u64(disc);
    idx_t r = (root + 1) / 2;                   /* row number */

    /* Compute rows until we have reached row r. */
    while (cur_n < r) {
        size_t next_len = cur_n + 1;            /* number of coefficients of next row */

        ensure_capacity(next_len);
        if (!u || !v || !tmp_u || !tmp_v) return 0;  /* allocation failed */

        /* Compute the next row into tmp arrays. */
        for (idx_t i = 0; i < next_len; ++i) {
            seq_t u_val = (i < cur_n) ? u[i] : 0;
            seq_t v_val = (i > 0)     ? v[i-1] : 0;
            tmp_u[i] = u_val + (seq_t)2 * v_val;

            seq_t u_prev = (i > 0) ? u[i-1] : 0;
            seq_t w = u_val + u_prev;
            seq_t v_curr = (i < cur_n) ? v[i] : 0;
            tmp_v[i] = w + v_curr + (i == 0 ? 1 : 0);
        }

        /* Swap pointers: tmp arrays become the current ones. */
        seq_t *swap = u; u   = tmp_u;  tmp_u = swap;
        swap       = v; v   = tmp_v;  tmp_v = swap;

        cur_n = next_len;
    }

    /* Compute the position inside row r. */
    idx_t start_of_row = (r * (r - 1)) / 2;   /* terms before row r */
    idx_t pos = t - start_of_row;

    return u[pos];
}
