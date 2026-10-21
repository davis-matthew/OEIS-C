/*
 * A209773  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209774; see the Formula section.
 * https://oeis.org/A209773
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the coefficient of x^k in u(r,x) where r is the row with
 *            * r(r+1)/2 ≥ n and k = n − r(r−1)/2 − 1, with * u(1,x)=1,
 *            v(1,x)=1 * u(n,x)=x*u(n−1,x)+(x+1)*v(n−1,x) *
 *            v(n,x)=(x+1)*u(n−1,x)+2x*v(n−1,x)+1
 * Range:     u8 n <= 24, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Find the row r and the position k within that row. */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;
    idx_t prev_tri = r * (r - 1) / 2;
    idx_t k = n - prev_tri - 1;          /* 0‑based index inside row r */

    /* Temporary storage for two successive rows of u and v.
       The maximum row needed for n ≤ 10 000 is 141, so 200 is safe. */
    static seq_t u_prev_arr[200];
    static seq_t v_prev_arr[200];
    static seq_t u_curr_arr[200];
    static seq_t v_curr_arr[200];

    /* pointer variables to the current arrays. */
    seq_t *u_prev = u_prev_arr;
    seq_t *v_prev = v_prev_arr;
    seq_t *u_curr = u_curr_arr;
    seq_t *v_curr = v_curr_arr;

    /* Initialise base case u(1,x)=v(1,x)=1. */
    u_prev[0] = (seq_t)1;
    v_prev[0] = (seq_t)1;

    /* Build rows 2 … r. */
    for (idx_t row = 1; row < r; ++row)   /* row holds n‑1 at this point */
    {
        idx_t sz = row + 1;               /* size of the next row */

        /* Compute next u coefficients: u(row+1,x) */
        u_curr[0] = v_prev[0];
        for (idx_t i = 1; i < sz; ++i) {
            seq_t val = u_prev[i - 1] + v_prev[i - 1];
            if (i < row)                  /* v_prev has length 'row' */
                val += v_prev[i];
            u_curr[i] = val;
        }

        /* Compute next v coefficients: v(row+1,x) */
        v_curr[0] = u_prev[0] + (seq_t)1;
        for (idx_t i = 1; i < sz; ++i) {
            seq_t val = u_prev[i - 1];
            if (i < row)                  /* u_prev has length 'row' */
                val += u_prev[i];
            val += (seq_t)2 * v_prev[i - 1];
            v_curr[i] = val;
        }

        /* Swap pointers for the next iteration. */
        seq_t *tmp = u_prev;  u_prev = u_curr;  u_curr = tmp;
        tmp = v_prev;         v_prev = v_curr;  v_curr = tmp;
    }

    /* u_prev now holds the coefficients of row r. */
    return u_prev[k];
}
