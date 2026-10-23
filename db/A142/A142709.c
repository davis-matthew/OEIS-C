/*
 * A142709  Polynomials made from cycloidal standing waves: p(x,n) = (4*n +
 *          2)*x + x^n.
 * https://oeis.org/A142709
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned, tabf
 * Formula:   a(n) is the row‑major enumeration of the triangle of
 *            coefficients of the cycloidal wave polynomial p(x,r) = (4r+2)x +
 *            x^r. Row 0 has coefficients [1,2]. For r ≥ 1 the row has length
 *            r+1 with t(r,0) = 0 t(r,1) = 4r+2 (for r=1 this is 7 because of
 *            the extra x^1 term) t(r,r) = 1 (for r>1) All other...
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Find row r and cumulative count of elements before row r */
    idx_t r = 0;
    idx_t cum = 0;          /* cumulative count before current row */
    idx_t len;
    while (1) {
        if (r == 0) len = 2;
        else if (r == 1) len = 2;
        else len = r + 1;   /* for r >= 2 */
        if (n <= cum + len) break;
        cum += len;
        r++;
    }
    idx_t c = n - cum - 1;   /* zero‑based column index in row r */

    /* Compute the triangle entry t(r,c).  */
    if (r == 0) {
        if (c == 0) return (seq_t)1;
        if (c == 1) return (seq_t)2;
        return (seq_t)0;      /* should not occur */
    }
    if (c == 0) return (seq_t)0;
    if (c == 1) {
        /* For r==1 the x^1 term appears twice, giving 7.  */
        if (r == 1) return (seq_t)7;
        return (seq_t)(4 * (seq_t)r + 2);
    }
    if (c == r) return (seq_t)1;
    return (seq_t)0;
}
