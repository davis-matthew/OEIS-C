/*
 * A223901  Poly-Cauchy numbers of the second kind hat c_n^(-3).
 * https://oeis.org/A223901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} Stirling1(n,k) * (-1)^k * (k+1)^3. Efficient
 *            implementation: a(n) = (-1)^n * [1*|Stirling1(n+1,1)| +
 *            7*|Stirling1(n+1,2)| + 12*|Stirling1(n+1,3)| +
 *            6*|Stirling1(n+1,4)|]
 * Range:     i8 n <= 2, i16 n <= 5, i32 n <= 10, i64 n <= 18, i128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* current Stirling numbers s(n,k) for k=0..4 (signed).  */
    seq_t s[5] = { 0 };
    s[0] = 1;                     /* s(0,0)=1 */
    /* coefficient array for k=1..4: 1,7,12,6. */
    const seq_t coef[5] = { 0, 1, 7, 12, 6 };

    seq_t next[5];
    for (idx_t i = 0; i <= n; ++i) {
        /* compute s(i+1, k) from s(i, k) */
        next[0] = 0;                       /* s(·,0)=0 for n>0 */
        for (int k = 1; k <= 4; ++k) {
            next[k] = s[k - 1] - (seq_t)i * s[k];
        }
        /* copy to s for next iteration */
        for (int k = 0; k <= 4; ++k) {
            s[k] = next[k];
        }

        /* compute a(i) using s(i+1, k) */
        seq_t val = 0;
        seq_t sign_n = ((i & 1) == 0) ? 1 : -1;        /* (-1)^i */
        for (int k = 1; k <= 4; ++k) {
            /* |s(i+1,k)| = (-1)^((i+1)-k) * s[i+1][k] */
            seq_t sign_abs = (((i + 1 - k) & 1) == 0) ? 1 : -1;
            seq_t abs_s   = sign_abs * s[k];
            val += sign_n * coef[k] * abs_s;
        }
        if (i == n) {
            return val;
        }
    }
    /* unreachable */
    return 0;
}
