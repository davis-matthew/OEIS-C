/*
 * A210552  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210553; see the Formula section.
 * https://oeis.org/A210552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the coefficient of x^(c-1) in the polynomial u(r,x),
 *            where r is the smallest integer with r(r+1)/2 ≥ n and c = n –
 *            r(r-1)/2. The polynomials u and v are defined by u(1,x) = 1 ,
 *            v(1,x) = 1 u(n,x) = x*u(n-1,x) + x*v(n-1,x) + 1 v(n,x) =
 *            x*u(n-1,x) + v(n-1,x) + 1 The triangle formed by the co...
 * Range:     u8 n <= 61, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t **U_rows = NULL;   /* coefficients of u(n,x) */
static seq_t **V_rows = NULL;   /* coefficients of v(n,x) */
static idx_t  max_row = 0;      /* highest row that has been computed */

/* Ensure that rows up to `up_to` (inclusive) are computed and stored. */
static void ensure_rows(idx_t up_to)
{
    if (up_to <= max_row)
        return;

    for (idx_t r = max_row + 1; r <= up_to; ++r)
    {
        /* Resize the pointer arrays to accommodate new row `r`. */
        size_t new_cap = r + 1;                     /* indices 0..r   */
        U_rows = (seq_t**)realloc(U_rows, new_cap * sizeof(seq_t*));
        V_rows = (seq_t**)realloc(V_rows, new_cap * sizeof(seq_t*));

        /* Allocate space for the current row.  Index 0 is unused. */
        U_rows[r] = (seq_t*)malloc((r + 1) * sizeof(seq_t));
        V_rows[r] = (seq_t*)malloc((r + 1) * sizeof(seq_t));

        if (r == 1)
        {
            U_rows[1][1] = 1;
            V_rows[1][1] = 1;
        }
        else
        {
            U_rows[r][1] = 1;                      /* constant term */
            V_rows[r][1] = V_rows[r-1][1] + 1;      /* grows by 1 each step */

            for (idx_t k = 2; k <= r; ++k)
            {
                seq_t u_prev = U_rows[r-1][k-1];
                seq_t v_prev_left = V_rows[r-1][k-1];      /* for U */
                seq_t v_prev_right = (k <= r-1) ? V_rows[r-1][k] : 0; /* for V */

                U_rows[r][k] = u_prev + v_prev_left;
                V_rows[r][k] = u_prev + v_prev_right;
            }
        }
    }
    max_row = up_to;
}

seq_t a(idx_t n)
{
    if (n < 1) /* undefined, but guard against invalid input */
        return 0;

    /* Find the row `r` such that r(r+1)/2 >= n. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* Ensure rows up to `r` are computed. */
    ensure_rows(r);

    /* Compute the column index within the row. */
    idx_t prev_terms = (r - 1) * r / 2;
    idx_t c = n - prev_terms;   /* 1-based column */

    return U_rows[r][c];
}
