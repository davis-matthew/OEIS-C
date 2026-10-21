/*
 * A209769  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209770; see the Formula section.
 * https://oeis.org/A209769
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Triangle of coefficients of polynomials u(n,x): u(1,x)=1,
 *            v(1,x)=1 u(n,x)=x*u(n-1,x)+(x+1)*v(n-1,x)
 *            v(n,x)=(x+1)*u(n-1,x)+v(n-1,x)+1 The flattened triangle gives
 *            a(1)=1, a(2)=1, a(3)=2, …
 * Range:     u8 n <= 24, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static arrays storing each row of the polynomials */
static seq_t **u_rows = NULL;     /* u_rows[r-1] points to array of r coeffs	*/
static seq_t **v_rows = NULL;     /* v_rows[r-1] points to array of r coeffs	*/
static size_t stored_rows = 0;    /* number of rows already computed */
static size_t stored_cap  = 0;    /* capacity of the row pointer arrays */

/* ensure at least R rows are computed; compute missing rows */
static void ensure_rows(size_t R)
{
    size_t i;
    if (R <= stored_rows)
        return;                       /* nothing to do */

    /* grow the pointer arrays as needed */
    if (stored_cap < R) {
        size_t new_cap = stored_cap ? stored_cap * 2 : 4;
        if (new_cap < R) new_cap = R;
        u_rows = (seq_t**)realloc(u_rows, new_cap * sizeof(seq_t*));
        v_rows = (seq_t**)realloc(v_rows, new_cap * sizeof(seq_t*));
        stored_cap = new_cap;
    }

    /* compute rows from stored_rows+1 up to R */
    for (i = stored_rows; i < R; ++i) {
        size_t r = i + 1;                   /* 1‑based row number */
        size_t prev_len = r - 1;             /* length of previous row */

        /* allocate storage for the current row and clear it */
        seq_t *u_curr = (seq_t*)malloc(r * sizeof(seq_t));
        seq_t *v_curr = (seq_t*)malloc(r * sizeof(seq_t));
        for (size_t j = 0; j < r; ++j) { u_curr[j] = 0; v_curr[j] = 0; }

        if (r == 1) {                       /* base row */
            u_curr[0] = (seq_t)1;
            v_curr[0] = (seq_t)1;
        } else {
            seq_t *u_prev = u_rows[i - 1];
            seq_t *v_prev = v_rows[i - 1];

            for (size_t j = 0; j < prev_len; ++j) {
                /* u(n,x) = x*u(n-1,x)+(x+1)*v(n-1,x) */
                u_curr[j + 1] += u_prev[j] + v_prev[j];   /* x*term + x*v */
                u_curr[j]     += v_prev[j];               /* constant term */

                /* v(n,x) = (x+1)*u(n-1,x)+v(n-1,x)+1 */
                v_curr[j + 1] += u_prev[j];               /* x*u term */
                v_curr[j]     += u_prev[j] + v_prev[j];   /* u + v term */
            }
            v_curr[0] += (seq_t)1;                           /* +1 constant */
        }

        /* store the computed row */
        u_rows[i] = u_curr;
        v_rows[i] = v_curr;
    }

    stored_rows = R;
}

/* Return the nth term a(n) of the sequence (n ≥ 1) */
seq_t a(idx_t n)
{
    /* find the row r such that 1 + 2 + … + (r-1) < n ≤ 1 + 2 + … + r */
    size_t r = 1;
    idx_t cumulative = 0;
    while (cumulative + (idx_t)r < n) {
        cumulative += (idx_t)r;
        ++r;
    }

    /* ensure all rows up to r are available */
    ensure_rows(r);

    /* index within the row (1‑based) */
    idx_t k = n - cumulative;               /* 1 ≤ k ≤ r */
    size_t coeff_index = (size_t)(k - 1);    /* 0‑based array index */

    return u_rows[r - 1][coeff_index];
}
