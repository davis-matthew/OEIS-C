/*
 * A172214  Number of ways to place 5 nonattacking knights on a 5 X n board.
 * https://oeis.org/A172214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1}^{13} c_k · binom(n-k+5,5) * where c = [1, 22,
 *            106, 814, 918, -720, 2834, -1188, -268, * 884, -268, -52, 42]
 *            are the coefficients of *
 *            x·(1+22x+106x²+814x³+918x⁴-720x⁵+2834x⁶-1188x⁷ *
 *            -268x⁸+884x⁹-268x¹⁰-52x¹¹+42x¹²)/(1-x)⁶
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 46, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* the 13 non‑zero coefficients of the numerator polynomial after
       multiplying by x.  coeff[0] is unused to keep indices 1‑13.    */
    static const int coeff[14] = {
        0,  /* unused  */
        1, 22, 106, 814, 918, -720, 2834, -1188, -268,
        884, -268, -52, 42
    };

    if (n == 0)     /* offset is 1; return 0 for n==0 (unused) */
        return 0;

    seq_t S_plus  = 0;   /* sum of terms with positive coefficients */
    seq_t S_minus = 0;   /* sum of absolute values of terms with negative coefficients */

    for (idx_t k = 1; k <= 13; ++k) {
        if (n < k)          /* terms with n-k+5 < 5 contribute 0 */
            continue;

        /* compute binom(n-k+5,5) as (m*(m-1)*(m-2)*(m-3)*(m-4))/120   */
        idx_t m = n - k + 5;        /* m >= 5 because n >= k                   */
        seq_t c = (seq_t)m * (m-1) * (m-2) * (m-3) * (m-4) / 120;

        int cv = coeff[k];
        if (cv > 0)
            S_plus += c * (seq_t)cv;
        else
            S_minus += c * (seq_t)(-cv);
    }

    /* final result: positive sum minus negative sum (the result is non‑negative) */
    return S_plus - S_minus;
}
