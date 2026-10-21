/*
 * A208328  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A208329; see the Formula section.
 * https://oeis.org/A208328
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 43, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the coefficients of u(n,x) (see A208328)
   u(n,x) = u(n-1,x) + x*v(n-1,x)
   v(n,x) = 2x*u(n-1,x) + x*v(n-1,x)
   with u(1,x)=1, v(1,x)=1.
   a(n) is obtained by flattening the coefficient arrays of u
   row‑major:  row 1 → 1; row 2 → 1,1; row 3 → 1,1,3; … */

/* To avoid dynamic allocation we keep static buffers large enough
   for all rows up to n≈10 000 (rows ≤ 142). */
#define MAX_ROWS 2000

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;           /* undefined for n < 1 */

    /* --- find the row that contains the n‑th element --- */
    idx_t r = 1;
    while (((r * (r + 1U)) / 2U) < n)
        ++r;

    /* --- buffers for the current and next rows --- */
    static seq_t u[MAX_ROWS];
    static seq_t v[MAX_ROWS];
    static seq_t u_next[MAX_ROWS];
    static seq_t v_next[MAX_ROWS];
    idx_t len = 1;          /* current row length (row index) */

    u[0] = (seq_t)1;
    v[0] = (seq_t)1;        /* v(1,x) = 1, although it is not needed after r = 1 */

    if (r > 1) {
        for (idx_t row = 2; row <= r; ++row) {
            /* compute u_next[0] and v_next[0] */
            u_next[0] = u[0];
            v_next[0] = (seq_t)0;

            /* compute remaining coefficients of the new row */
            for (idx_t k = 1; k < row; ++k) {
                seq_t term_u = (k < len) ? u[k] : (seq_t)0;   /* u(row-1,k) */
                seq_t term_v = v[k - 1];                     /* v(row-1,k-1) */

                u_next[k] = term_u + term_v;
                v_next[k] = (seq_t)2 * u[k - 1] + term_v;     /* 2*u(row-1,k-1)+v(row-1,k-1) */
            }

            /* copy next row into current arrays for next iteration */
            for (idx_t k = 0; k < row; ++k) {
                u[k] = u_next[k];
                v[k] = v_next[k];
            }

            len = row;
        }
    }

    /* index of the requested element within the computed row (0‑based) */
    idx_t prev_sum = ((r - 1U) * r) / 2U;    /* elements before row r */
    idx_t col = n - prev_sum - 1U;           /* column in row r */

    return u[col];
}
