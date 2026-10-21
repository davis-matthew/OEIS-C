/*
 * A208918  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A208917; see the Formula section.
 * https://oeis.org/A208918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = coefficient of v(r,x) at degree k‑1, where r is the * row
 *            containing the nth entry of the flattened triangle, and the *
 *            coefficients of v(n,x) are obtained by the recurrence * * u(1,x)
 *            = 1 v(1,x) = 1 * u(n,x) = u(n‑1,x) + 2x·v(n‑1,x) * v(n,x) =
 *            2x·u(n‑1,x) + 2x·v(n‑1,x) + 1 * * a(1)=1, a(2...
 * Range:     u8 n <= 20, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r and column k (1‑based) that contains the term
       a(n).  Zero‑based offset t = n−1.  After the loop
       t < r, and r is the desired row. */
    idx_t t = n - ((idx_t)1);
    idx_t r = (idx_t)1;
    while (t >= r) {
        t -= r;
        ++r;
    }
    idx_t k = t + ((idx_t)1);   /* 1‑based column within row r */

    /* Max row needed for n up to about 10 000 is < 150.
       Use a statically allocated buffer that covers that safely. */
    enum { MAX_R = 200 };       /* upper bound on r for prescribed limits */

    static seq_t u_prev[MAX_R];
    static seq_t v_prev[MAX_R];
    static seq_t u_cur[MAX_R];
    static seq_t v_cur[MAX_R];

    /* Initial polynomials for n = 1. */
    u_prev[0] = (seq_t)1;
    v_prev[0] = (seq_t)1;
    idx_t len_prev = (idx_t)1;

    /* Build rows up to r. */
    for (idx_t cur = (idx_t)2; cur <= r; ++cur) {
        idx_t len_cur = cur;

        /* Zero the current buffers only up to len_cur. */
        for (idx_t j = (idx_t)0; j < len_cur; ++j) {
            u_cur[j] = (seq_t)0;
            v_cur[j] = (seq_t)0;
        }

        /* Recurrence for u(cur,x) and v(cur,x). */
        for (idx_t j = (idx_t)0; j < len_prev; ++j) {
            /* u(cur,x) = u(prev,x) + 2x·v(prev,x)   (shifted by 1) */
            u_cur[j] += u_prev[j];
            u_cur[j + ((idx_t)1)] += (seq_t)2 * v_prev[j];

            /* v(cur,x) = 1 + 2x·u(prev,x) + 2x·v(prev,x) */
            v_cur[j + ((idx_t)1)] += (seq_t)2 * u_prev[j] +
                                     (seq_t)2 * v_prev[j];
        }
        v_cur[0] += (seq_t)1;   /* constant term of v(cur,x) */

        /* Copy current polynomials to previous buffers for next iteration. */
        for (idx_t j = (idx_t)0; j < len_cur; ++j) {
            u_prev[j] = u_cur[j];
            v_prev[j] = v_cur[j];
        }
        len_prev = len_cur;
    }

    /* Return the requested coefficient (k‑th entry of row r). */
    return v_prev[k - ((idx_t)1)];
}
