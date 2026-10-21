/*
 * A209565  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209566; see the Formula section.
 * https://oeis.org/A209565
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle of coefficients of u(n,x); a(n) is the nth element of the
   row‑major flattening of the triangle
   u(n,x)=x*u(n‑1,x)+v(n‑1,x),
   v(n,x)=x*u(n‑1,x)+2x*v(n‑1,x)+1,
   u(1,x)=v(1,x)=1. */
#include <stdlib.h>
#include <string.h>

/* static storage for generated rows */
static seq_t *coeffbuf = NULL;          /* flattened coefficients */
static size_t coeffcap = 0;              /* allocated capacity of coeffbuf */
static idx_t max_row = 0;                /* highest row computed (1‑based) */

static seq_t *u_prev = NULL;            /* coefficients of u for previous row */
static seq_t *v_prev = NULL;            /* coefficients of v for previous row */
static size_t prev_len = 0;              /* length of previous row */

/* grow coeffbuf to at least needed capacity */
static void ensure_coeffbuf(size_t needed)
{
    if (needed <= coeffcap)
        return;
    size_t newcap = coeffcap ? coeffcap << 1 : 128;
    while (newcap < needed)
        newcap <<= 1;
    seq_t *tmp = (seq_t *)realloc(coeffbuf, newcap * sizeof(seq_t));
    if (!tmp)
        abort();                       /* allocation failure */
    coeffbuf = tmp;
    coeffcap = newcap;
}

/* compute rows up to target_row inclusive */
static void compute_rows_up_to(idx_t target_row)
{
    if (max_row >= target_row)
        return;

    if (max_row == 0) {                     /* initialise row 1 */
        u_prev = (seq_t *)malloc(sizeof(seq_t));
        v_prev = (seq_t *)malloc(sizeof(seq_t));
        if (!u_prev || !v_prev)
            abort();
        prev_len = 1;
        u_prev[0] = 1;
        v_prev[0] = 1;
        ensure_coeffbuf(1);
        coeffbuf[0] = 1;                   /* u(1,x)=1 */
        max_row = 1;
    }

    for (idx_t row = max_row + 1; row <= target_row; ++row) {
        size_t len_prev = row - 1;
        size_t len_curr = row;             /* new row has one more coefficient */

        seq_t *u_curr = (seq_t *)malloc(sizeof(seq_t) * len_curr);
        seq_t *v_curr = (seq_t *)malloc(sizeof(seq_t) * len_curr);
        if (!u_curr || !v_curr)
            abort();

        for (size_t k = 0; k < len_curr; ++k) {
            seq_t a_u = (k > 0) ? u_prev[k - 1] : 0;
            seq_t a_v = (k < len_prev) ? v_prev[k] : 0;
            /* u(n) = x*u_prev + v_prev */
            u_curr[k] = a_u + a_v;

            seq_t b_u = (k > 0) ? u_prev[k - 1] : 0;
            seq_t b_v = (k > 0) ? (seq_t)2 * v_prev[k - 1] : 0;
            seq_t c   = (k == 0) ? 1 : 0;
            /* v(n) = x*u_prev + 2x*v_prev + 1 */
            v_curr[k] = b_u + b_v + c;
        }

        /* store coefficients of u_curr into the flat buffer */
        size_t offset = (row - 1) * row / 2;   /* T_{row-1} */
        ensure_coeffbuf(offset + len_curr);
        memcpy(coeffbuf + offset, u_curr, len_curr * sizeof(seq_t));

        /* free previous row arrays */
        free(u_prev);
        free(v_prev);

        /* promote current arrays to previous for next iteration */
        u_prev = u_curr;
        v_prev = v_curr;
        prev_len = len_curr;
        max_row = row;
    }
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                           /* undefined for offset<1 */

    /* find the row that contains index n */
    idx_t r = 1;
    while ((seq_t)r * (r + 1) / 2 < n)
        ++r;

    compute_rows_up_to(r);

    return coeffbuf[n - 1];
}
