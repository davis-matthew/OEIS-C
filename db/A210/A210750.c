/*
 * A210750  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210749; see the Formula section.
 * https://oeis.org/A210750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the polynomials
 *   u(1,x)=1,         v(1,x)=1
 *   u(n,x) = (x+1)*u(n-1,x) + v(n-1,x) + 1
 *   v(n,x) = (x+1)*u(n-1,x) + 2x*v(n-1,x) + 1
 * The triangle is formed by the coefficients of v(n,x) (constant term first).
 * a(n) is the n-th element of this triangle when read row‑wise from
 * n = 1.  (Row r contains r coefficients.)
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

/* Compute the row (r) and position (k, 0‑based) corresponding to the
 * overall index n.  Row 1 has 1 element, row 2 has 2, etc. */
static void row_and_pos(idx_t n, idx_t *row, idx_t *pos)
{
    /* find minimal r such that r*(r+1)/2 >= n  */
    idx_t low = 1;
    /* upper bound using sqrt; safe for very large n */
    long double approx = sqrtl( (long double)2.0 * (long double)n );
    idx_t high = (idx_t)(approx + 3.0L);  /* add a safety margin */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 sum = (unsigned __int128)mid * (mid + 1) / 2;
        if (sum >= (unsigned __int128)n)
            high = mid;
        else
            low = mid + 1;
    }
    *row = low;
    /* previous total elements before this row */
    unsigned __int128 prev_total = (unsigned __int128)(low - 1) * low / 2;
    *pos = (idx_t)( (unsigned __int128)n - prev_total - 1 );  /* 0‑based */
}

/* Main function requested by the build system. */
seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;   /* first element of the triangle */

    idx_t r, k;
    row_and_pos(n, &r, &k);

    /* Allocate coefficient arrays for the current row.
     * We keep two sets of arrays: previous (u_prev, v_prev) and current. */
    seq_t *u_prev = (seq_t *)malloc(sizeof(seq_t));
    seq_t *v_prev = (seq_t *)malloc(sizeof(seq_t));
    if (!u_prev || !v_prev) return (seq_t)0; /* allocation failed */

    u_prev[0] = (seq_t)1;
    v_prev[0] = (seq_t)1;

    for (idx_t cur = 2; cur <= r; ++cur) {
        idx_t sz_cur = cur;          /* new arrays have size cur  */
        seq_t *u_cur = (seq_t *)malloc(sz_cur * sizeof(seq_t));
        seq_t *v_cur = (seq_t *)malloc(sz_cur * sizeof(seq_t));
        if (!u_cur || !v_cur) {
            free(u_prev); free(v_prev);
            return (seq_t)0;
        }

        /* constant term (degree 0) */
        u_cur[0] = u_prev[0] + v_prev[0] + (seq_t)1;
        v_cur[0] = u_prev[0] + (seq_t)1;

        /* middle terms */
        for (idx_t j = 1; j < sz_cur - 1; ++j) {
            /* u term */
            u_cur[j] = u_prev[j] + v_prev[j] + u_prev[j - 1];

            /* v term: 2*v_prev[j-1] is written as v_prev[j-1] + v_prev[j-1] */
            seq_t two_v = v_prev[j - 1] + v_prev[j - 1];
            v_cur[j]   = u_prev[j] + u_prev[j - 1] + two_v;
        }

        /* last degree (sz_cur-1) */
        u_cur[sz_cur - 1] = u_prev[sz_cur - 2];
        seq_t two_v_last  = v_prev[sz_cur - 2] + v_prev[sz_cur - 2];
        v_cur[sz_cur - 1] = u_prev[sz_cur - 2] + two_v_last;

        free(u_prev);
        free(v_prev);
        u_prev = u_cur;
        v_prev = v_cur;
    }

    seq_t result = v_prev[k];
    free(u_prev);
    free(v_prev);
    return result;
}
