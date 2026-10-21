/*
 * A210559  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210560; see the Formula section.
 * https://oeis.org/A210559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the coefficient of x^(k-1) in the polynomial u(r,x),
 *            where r is the unique integer with r(r-1)/2 < n ≤ r(r+1)/2, k =
 *            n - r(r-1)/2. Recurrence: u(1,x) = 1, v(1,x) = 1 u(n,x) =
 *            x*u(n-1,x) + x*v(n-1,x) + 1 v(n,x) = (x+1)*u(n-1,x) + v(n-1,x) +
 *            1
 * Range:     u8 n <= 40, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   allocate_seq_row
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/allocate_seq_row.h"

static seq_t **u_rows = NULL;     /* u_rows[r][i] = coefficient of x^i in u(r,x) */
static seq_t **v_rows = NULL;     /* v_rows[r][i] = coefficient of x^i in v(r,x) */
static idx_t  max_row = 0;        /* highest row currently stored */

/* Ensure that rows up to 'need' are computed and stored. */
static void ensure_rows(idx_t need)
{
    if (need <= max_row)
        return;

    u_rows = (seq_t**)realloc(u_rows, sizeof(seq_t*) * (need + 1));
    v_rows = (seq_t**)realloc(v_rows, sizeof(seq_t*) * (need + 1));

    for (idx_t r = max_row + 1; r <= need; ++r)
    {
        seq_t *u_cur = allocate_seq_row(r);
        seq_t *v_cur = allocate_seq_row(r);
        u_rows[r] = u_cur;
        v_rows[r] = v_cur;

        if (r == 1)
        {
            u_cur[0] = (seq_t)1;
            v_cur[0] = (seq_t)1;
        }
        else
        {
            idx_t prev = r - 1;
            seq_t *u_prev = u_rows[prev];
            seq_t *v_prev = v_rows[prev];

            /* u(r,x) = x*u(r-1,x) + x*v(r-1,x) + 1 */
            u_cur[0] = (seq_t)1;
            for (idx_t k = 1; k < r; ++k)
                u_cur[k] = u_prev[k-1] + v_prev[k-1];

            /* v(r,x) = (x+1)*u(r-1,x) + v(r-1,x) + 1 */
            v_cur[0] = u_prev[0] + v_prev[0] + (seq_t)1;
            for (idx_t k = 1; k < r; ++k)
            {
                seq_t up_k   = (k <= prev-1) ? u_prev[k]   : (seq_t)0;
                seq_t up_k_1 = u_prev[k-1];
                seq_t vp_k   = (k <= prev-1) ? v_prev[k]   : (seq_t)0;
                v_cur[k] = up_k + up_k_1 + vp_k;
            }
        }
    }
    max_row = need;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined, but safe for callers */

    idx_t r = 1;
    idx_t s = 1;            /* s = r(r+1)/2, number of terms up to row r */
    while (s < n) {
        ++r;
        s = r * (r + 1) / 2;
    }

    idx_t prev_sum = (r - 1) * r / 2;   /* terms before row r */
    idx_t k = n - prev_sum;             /* 1‑based column index within the row */

    ensure_rows(r);
    return u_rows[r][k - 1];
}
