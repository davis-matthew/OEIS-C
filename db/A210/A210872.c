/*
 * A210872  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210873; see the Formula section.
 * https://oeis.org/A210872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the coefficient of x^{j} in the polynomial u(r,x), where
 *            r is the unique integer with (r-1)r/2 < n ≤ r(r+1)/2 and j = n -
 *            (r-1)r/2 - 1. The polynomials u satisfy u(1,x)=1,
 *            u(n,x)=x*u(n-1,x)+v(n-1,x)-1, v(1,x)=1,
 *            v(n,x)=x*u(n-1,x)+x*v(n-1,x)+1. This function builds the rows of
 *            u iterativel...
 * Range:     u8 n <= 62, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Forward declarations of types supplied by the build */
extern seq_t a(idx_t n);

/* Static storage for generated rows of u */
static seq_t **u_rows = NULL;    /* u_rows[r] points to array of length r */
static idx_t  max_row = 0;       /* largest row currently stored */
static idx_t  rows_cap = 0;      /* capacity of rows array */

/* Previous rows needed for recurrence */
static seq_t *prev_u = NULL;     /* u(n-1,x) */
static seq_t *prev_v = NULL;     /* v(n-1,x) */

/* Find r such that (r-1)*r/2 < n ≤ r*(r+1)/2 */
static idx_t row_for_idx(idx_t n)
{
    idx_t low = 0, high = 1;
    /* exponentially find an upper bound */
    while (((unsigned __int128)high * (high + 1) / 2) < n)
        high <<= 1;
    /* binary search */
    while (low < high) {
        idx_t mid = low + ((high - low) >> 1);
        if (((unsigned __int128)mid * (mid + 1) / 2) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* Ensure that rows 1..r are computed and stored. */
static void ensure_rows(idx_t r)
{
    if (r <= max_row)
        return;

    /* Allocate or enlarge the row pointer array */
    if (u_rows == NULL) {
        u_rows = (seq_t**)malloc((r + 1) * sizeof(seq_t*));
        for (idx_t i = 0; i <= r; ++i)
            u_rows[i] = NULL;
        rows_cap = r + 1;
    } else if (r + 1 > rows_cap) {
        size_t new_cap = rows_cap * 2;
        if (new_cap < r + 1)
            new_cap = r + 1;
        seq_t **tmp = (seq_t**)realloc(u_rows, new_cap * sizeof(seq_t*));
        if (!tmp)
            abort();
        for (idx_t i = rows_cap; i < new_cap; ++i)
            tmp[i] = NULL;
        u_rows = tmp;
        rows_cap = new_cap;
    }

    /* Generate new rows up to r */
    for (idx_t cur = max_row + 1; cur <= r; ++cur) {
        /* allocate arrays for current u and v */
        seq_t *u_cur = (seq_t*)malloc(cur * sizeof(seq_t));
        if (!u_cur)
            abort();
        seq_t *v_cur = (seq_t*)malloc(cur * sizeof(seq_t));
        if (!v_cur)
            abort();

        if (cur == 1) {
            /* Base case: u(1)=1, v(1)=1 */
            u_cur[0] = (seq_t)1;
            v_cur[0] = (seq_t)1;
        } else {
            /* First coefficient is always zero for u, constant term of v is 1 */
            u_cur[0] = (seq_t)0;
            v_cur[0] = (seq_t)1;

            /* Fill remaining coefficients */
            for (idx_t j = 1; j < cur; ++j) {
                /* u(cur,j) = u(cur-1,j-1) + v(cur-1,j) */
                seq_t sum_u = prev_u[j - 1];
                if (j < cur - 1)         /* v has length cur-1 */
                    sum_u += prev_v[j];
                u_cur[j] = sum_u;

                /* v(cur,j) = u(cur-1,j-1) + v(cur-1,j-1) */
                seq_t sum_v = prev_u[j - 1] + prev_v[j - 1];
                v_cur[j] = sum_v;
            }
        }

        /* Store the new u row */
        u_rows[cur] = u_cur;

        /* Free the old v array */
        if (prev_v)
            free(prev_v);

        /* Prepare for next iteration */
        prev_u = u_cur;
        prev_v = v_cur;

        max_row = cur;
    }
}

/* Main function required by the framework */
seq_t a(idx_t n)
{
    if (n == 0)  /* out of offset; defined but not used */
        return (seq_t)0;

    /* Determine row r and column j (0‑based) */
    idx_t r = row_for_idx(n);
    idx_t prev_count = (unsigned __int128)(r - 1) * r / 2;
    idx_t j = n - prev_count - 1;

    /* Generate rows up to r if necessary */
    ensure_rows(r);

    /* Retrieve the required coefficient */
    return u_rows[r][j];
}
