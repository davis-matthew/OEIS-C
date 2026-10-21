/*
 * A209563  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209564; see the Formula section.
 * https://oeis.org/A209563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient of x^(n - (k-1)*k/2 - 1) in u(k,x), where k =
 *            ceil((sqrt(8*n+1)-1)/2) is the row that contains the nth
 *            coefficient of the triangle of polynomials u(n,x)
 * Range:     u8 n <= 45, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* external types supplied by the build system */
extern idx_t _floor_isqrt_u64(idx_t n);

/* static storage for the triangle rows (1‑based index) */
static seq_t **u_rows = NULL;   /* array of pointers to rows   */
static size_t  u_rows_cap = 0;  /* allocated size of u_rows    */
static idx_t   max_row = 0;     /* highest computed row index   */

/* storage for the most recent row used to generate the next one */
static seq_t *prev_u  = NULL;
static seq_t *prev_v  = NULL;
static idx_t  prev_len = 0;

/* ensure the u_rows array has at least cap entries (1‑based) */
static void ensure_u_rows_cap(size_t cap)
{
    if (u_rows_cap < cap) {
        size_t new_cap = u_rows_cap ? u_rows_cap : 16;
        while (new_cap < cap) new_cap <<= 1;
        seq_t **tmp = (seq_t**)realloc(u_rows, (new_cap + 1) * sizeof(seq_t*));
        if (!tmp) abort();         /* serious failure */
        u_rows = tmp;
        u_rows_cap = new_cap;
    }
}

/* compute the row index k such that k*(k+1)/2 >= n */
static idx_t row_of_index(idx_t n)
{
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        /* mid may be large, but for the problem limits it is safe in 64‑bit */
        seq_t tri = ((seq_t)mid * (mid + 1)) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    return low;
}

/* compute rows up to needed_r, storing all u rows */
static void ensure_rows_up_to(idx_t needed_r)
{
    while (max_row < needed_r) {
        idx_t next_r = max_row + 1;                 /* next row number */
        size_t next_len = (size_t)next_r;           /* number of coeffs */

        /* allocate new row arrays */
        seq_t *u_new = (seq_t*)malloc(next_len * sizeof(seq_t));
        seq_t *v_new = (seq_t*)malloc(next_len * sizeof(seq_t));
        if (!u_new || !v_new) abort();            /* allocation failure */

        if (max_row == 0) {                       /* first row (r=1) */
            u_new[0] = (seq_t)1;
            v_new[0] = (seq_t)1;
        } else {
            /* compute u_new[k] and v_new[k] from prev_u and prev_v */
            /* k = 0 */
            u_new[0] = prev_v[0];
            v_new[0] = (seq_t)1;

            /* middle coefficients */
            for (size_t k = 1; k < prev_len; ++k) {
                u_new[k] = prev_u[k-1] + prev_v[k];
                v_new[k] = prev_u[k-1] + prev_v[k-1];
            }

            /* last coefficient of u_new */
            u_new[prev_len] = prev_u[prev_len-1];
            v_new[prev_len] = prev_u[prev_len-1] + prev_v[prev_len-1];
        }

        /* store the new u row */
        ensure_u_rows_cap(next_r);
        u_rows[next_r] = u_new;

        /* free the old v (no longer needed) */
        free(prev_v);

        /* prepare for next iteration */
        prev_u = u_new;
        prev_v = v_new;
        prev_len = next_len;
        max_row = next_r;
    }
}

/* public function requested by the problem statement */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;               /* undefined for n < 1 */

    idx_t row = row_of_index(n);
    ensure_rows_up_to(row);

    /* position inside the row */
    seq_t tri_before = ((seq_t)(row - 1) * row) / 2;
    idx_t pos = (idx_t)( (seq_t)n - tri_before - 1 );

    return u_rows[row][pos];
}
