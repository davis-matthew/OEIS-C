/*
 * A210858  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210859; see the Formula section.
 * https://oeis.org/A210858
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/*  Formula used:
        a(n) is the coefficient of x^k in u(r,x), where r is the
        row satisfying T_{r-1} < n ≤ T_r (T_r = r(r+1)/2).
        Polynomials u and v are defined by
            u(1,x) = 1 , v(1,x) = 1
            u(n,x) = u(n-1,x) + x*v(n-1,x)
            v(n,x) = (x + n)*u(n-1,x) + x*v(n-1,x)
        The flattened triangle index n is obtained by reading each row
        from left to right.  The function below computes the value
        directly from these recurrences.  No other closed formula is
        available for a(n). */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* sequence starts at n = 1 */

    /* --- Determine the row r and the column index pos ---------------- */

    idx_t r = 1;          /* current row number */
    idx_t total = 1;      /* T_r = r(r+1)/2 for current r */

    while (total < n) {
        ++r;
        total += r;       /* enlarge to next triangular number */
    }

    /* total now equals T_r, prev_total = T_{r-1} */
    idx_t prev_total = total - r;
    idx_t pos = n - prev_total - 1;   /* 0‑based position in row r */

    /* --- Allocate working arrays ------------------------------------- */

    seq_t *u     = (seq_t *)calloc(r, sizeof(seq_t));
    seq_t *v     = (seq_t *)calloc(r, sizeof(seq_t));
    seq_t *new_u = (seq_t *)calloc(r, sizeof(seq_t));
    seq_t *new_v = (seq_t *)calloc(r, sizeof(seq_t));

    if (!u || !v || !new_u || !new_v) {
        /* memory allocation failed */
        free(u); free(v); free(new_u); free(new_v);
        return 0;
    }

    /* --- Base row (row 1) -------------------------------------------- */
    u[0] = 1;
    v[0] = 1;

    /* --- Build rows 2 .. r ------------------------------------------- */
    for (idx_t i = 2; i <= r; ++i) {
        /* new_u[0] = u[0];                     (constant term)              */
        new_u[0] = u[0];

        /* new_v[0] = i * u[0];                 (constant term of v)        */
        new_v[0] = (seq_t)i * u[0];

        /* For k = 1 .. i-1 */
        for (idx_t k = 1; k < i; ++k) {
            new_u[k] = u[k] + v[k-1];
            new_v[k] = (seq_t)i * u[k] + u[k-1] + v[k-1];
        }

        /* swap the buffers for next iteration */
        seq_t *tmp = u;   u = new_u; new_u = tmp;
        tmp = v;         v = new_v; new_v = tmp;
    }

    /* --- Retrieve the requested coefficient -------------------------- */
    seq_t result = u[pos];

    /* --- Clean up ---------------------------------------------------- */
    free(u);
    free(v);
    free(new_u);
    free(new_v);

    return result;
}
