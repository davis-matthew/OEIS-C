/*
 * A209163  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209162; see the Formula section.
 * https://oeis.org/A209163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the k‑th coefficient (0‑based) of v(n,x) defined by
 *            u(1,x)=1, v(1,x)=1, u(n,x)=u(n‑1,x)+(x+1)*v(n‑1,x),
 *            v(n,x)=2x*u(n‑1,x)+(x+1)*v(n‑1,x)+1
 * Range:     u8 n <= 18, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* find the row m such that m*(m+1)/2 ≥ n (triangular numbers) */
    idx_t m = 1;
    seq_t tri = 1;                   /* T_1 */
    while ((seq_t)n > tri) {
        tri += ++m;                   /* T_m = T_{m-1} + m */
    }

    /* index of the coefficient within row m (0‑based) */
    idx_t k = n - ((m - 1) * m / 2) - 1;

    /* we need at most row 200 for n ≤ 10000 – safe bound */
    const idx_t MAX_ROW = 200;
    seq_t u_prev[MAX_ROW];
    seq_t v_prev[MAX_ROW];
    seq_t u_cur[MAX_ROW];
    seq_t v_cur[MAX_ROW];

    /* initialise the first row (n=1) */
    for (idx_t i = 0; i < MAX_ROW; ++i) {
        u_prev[i] = 0;
        v_prev[i] = 0;
    }
    u_prev[0] = 1;
    v_prev[0] = 1;
    idx_t prev_len = 1;                /* current length of previous row */

    /* generate rows 2 … m */
    for (idx_t row = 2; row <= m; ++row) {
        idx_t len = row;

        /* compute u(row, x) */
        for (idx_t i = 0; i < len; ++i) {
            seq_t a_u = (i < prev_len) ? u_prev[i] : 0;
            seq_t b_v = (i < prev_len) ? v_prev[i] : 0;
            seq_t c_v = (i > 0)          ? v_prev[i - 1] : 0;
            u_cur[i] = a_u + b_v + c_v;
        }

        /* compute v(row, x) */
        for (idx_t i = 0; i < len; ++i) {
            seq_t part1 = (i > 0) ? ((seq_t)2 * u_prev[i - 1]) : 0;
            seq_t part2 = (i < prev_len) ? v_prev[i] : 0;
            seq_t part3 = (i > 0)       ? v_prev[i - 1] : 0;
            seq_t const1 = (i == 0)     ? 1 : 0;
            v_cur[i] = part1 + part2 + part3 + const1;
        }

        /* copy current row to previous row for next iteration */
        for (idx_t i = 0; i < len; ++i) {
            u_prev[i] = u_cur[i];
            v_prev[i] = v_cur[i];
        }
        prev_len = len;
    }

    return v_prev[k];
}
