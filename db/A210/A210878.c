/*
 * A210878  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210879; see the Formula section.
 * https://oeis.org/A210878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) equals the n‑th entry of the flattened triangle of
 *            coefficients of the polynomials u(n,x) defined by u(1,x)=1,
 *            v(1,x)=1, u(n,x)=x*u(n-1,x)+2x*v(n-1,x),
 *            v(n,x)=(x+1)*u(n-1,x)+x*v(n-1,x)+1
 * Range:     u8 n <= 26, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Helper: find the triangle row r such that r*(r+1)/2 >= n. */
static idx_t _row_of(idx_t n)
{
    idx_t lo = 1, hi = 1;
    while ((__uint128_t)hi * (hi + 1) / 2 < n)
        hi <<= 1;                       /* exponential search for upper bound */

    while (lo < hi) {
        idx_t mid = (lo + hi) >> 1;
        if ((__uint128_t)mid * (mid + 1) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }
    return lo;
}

/* Structure storing previously computed rows of u and v coefficients. */
static idx_t _max_row = 0;            /* highest row already computed          */
static seq_t **_u_rows = NULL;        /* _u_rows[r] points to array of length r   */
static seq_t **_v_rows = NULL;        /* _v_rows[r] points to array of length r   */
static idx_t _rows_cap = 0;           /* allocated capacity for pointer arrays    */

/* Grow the pointer arrays to accommodate requested row. */
static void _ensure_capacity(idx_t needed)
{
    if (_rows_cap <= needed) {
        idx_t new_cap = (_rows_cap == 0) ? needed + 1 : _rows_cap * 2;
        if (new_cap < needed + 1)
            new_cap = needed + 1;
        _u_rows = (seq_t **)realloc(_u_rows, sizeof(seq_t *) * new_cap);
        _v_rows = (seq_t **)realloc(_v_rows, sizeof(seq_t *) * new_cap);
        _rows_cap = new_cap;
    }
}

/* Compute rows up to r (inclusive) if not already present. */
static void _ensure_rows(idx_t r)
{
    if (r <= _max_row)
        return;

    for (idx_t i = _max_row + 1; i <= r; ++i) {
        _ensure_capacity(i);

        seq_t *u_i = (seq_t *)malloc(sizeof(seq_t) * i);
        seq_t *v_i = (seq_t *)malloc(sizeof(seq_t) * i);
        if (!u_i || !v_i) {
            /* Handle allocation failure (not required by spec). */
            exit(1);
        }

        if (i == 1) {                 /* base case u(1)=1, v(1)=1 */
            u_i[0] = 1;
            v_i[0] = 1;
        } else {
            seq_t *prev_u = _u_rows[i - 1];
            seq_t *prev_v = _v_rows[i - 1];

            u_i[0] = 0;                              /* coefficient of x^0 */
            v_i[0] = prev_u[0] + 1;                  /* constant term of v */

            for (idx_t k = 1; k < i; ++k) {
                seq_t uprev = prev_u[k - 1];
                seq_t vprev = prev_v[k - 1];
                u_i[k] = uprev + (vprev + vprev);    /* uprev + 2*vprev */

                seq_t uprev_k = (k < i - 1) ? prev_u[k] : 0;
                v_i[k] = uprev_k + uprev + vprev;    /* prev_u[k] + prev_u[k-1] + prev_v[k-1] */
            }
        }

        _u_rows[i] = u_i;
        _v_rows[i] = v_i;
    }
    _max_row = r;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                              /* behavior below offset is irrelevant */

    idx_t r = _row_of(n);
    idx_t prev = (idx_t)((__uint128_t)r * (r - 1) / 2);
    idx_t k = n - prev - 1;                    /* 0‑based index within the row */

    _ensure_rows(r);
    return _u_rows[r][k];
}
