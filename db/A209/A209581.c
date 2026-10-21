/*
 * A209581  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209582; see the Formula section.
 * https://oeis.org/A209581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A209581: Triangle of coefficients of polynomials u(n,x)
   The sequence a(n) is the flattening of the triangle of coefficients
   of u(n,x).  For the given index n (1‑based) we find the row r such
   that T(r-1) < n ≤ T(r) where T(k)=k(k+1)/2.  Then c = n - T(r-1)
   (1‑based column) and a(n) = coefficient_{c-1} of u(r,x).

   u(1,x)=1 , v(1,x)=1
   u(n,x)=x*u(n-1,x)+v(n-1,x)
   v(n,x)=(x+1)*u(n-1,x)+2x*v(n-1,x)+1
*/

seq_t a(idx_t n)
{
    /* find row r and column index c (0‑based) */
    idx_t r = 1;
    idx_t tri = 1;                /* T(1) */
    while (tri < n) {
        ++r;
        tri += r;                 /* T(r) = T(r-1)+r */
    }
    idx_t prev_tri = tri - r;     /* T(r-1) */
    idx_t col = n - prev_tri - 1; /* 0‑based column */

    /* maximum row we will need for the given constraints.
       For n up to 10,000 this comfortably covers all cases. */
    const idx_t MAX_R = 2000;

    /* triangular arrays for current and previous rows */
    seq_t u_prev[MAX_R + 1];
    seq_t v_prev[MAX_R + 1];
    seq_t u_calc[MAX_R + 1];
    seq_t v_calc[MAX_R + 1];

    /* base case: row 1 */
    u_prev[0] = (seq_t)1;
    v_prev[0] = (seq_t)1;

    if (r == 1) {
        /* only one element in the row */
        return u_prev[col];
    }

    /* iterate rows from 2 up to r */
    for (idx_t row = 2; row <= r; ++row) {
        idx_t prev_len = row - 1;             /* length of previous row */

        /* compute u(row) */
        u_calc[0] = v_prev[0];
        for (idx_t i = 1; i < row; ++i) {
            seq_t term_v = (i < prev_len) ? v_prev[i] : (seq_t)0;
            u_calc[i] = term_v + u_prev[i - 1];
        }

        /* compute v(row) */
        v_calc[0] = u_prev[0] + (seq_t)1;
        for (idx_t i = 1; i < row; ++i) {
            seq_t term_u = (i <= prev_len - 1) ? u_prev[i] : (seq_t)0;
            seq_t term1 = u_prev[i - 1];
            seq_t term3 = (seq_t)2 * v_prev[i - 1];
            v_calc[i] = term1 + term_u + term3;
        }

        /* copy current row into previous row buffers for next iteration */
        for (idx_t i = 0; i < row; ++i) {
            u_prev[i] = u_calc[i];
            v_prev[i] = v_calc[i];
        }
    }

    /* after loop, u_prev holds the coefficients of row r */
    return u_prev[col];
}
