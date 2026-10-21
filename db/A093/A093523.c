/*
 * A093523  Inverse binomial transform of A010054 (1 if triangular number else
 *          0).
 * https://oeis.org/A093523
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   * a(n) = Σ_{m≥0, t=m(m+1)/2 ≤ n} (-1)^{n‑t} * C(n, t) * where
 *            C(n, t) is the binomial coefficient. * This is the inverse
 *            binomial transform of the indicator of triangular numbers
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 29, i64 n <= 36, i128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static seq_t binom(idx_t N, idx_t K)
{
    if (K > N)
        return 0;
    if (K > N - K)      /* use symmetry to reduce loop length */
        K = N - K;

    seq_t res = (seq_t)1;
    for (idx_t i = (idx_t)1; i <= K; ++i) {
        seq_t num = (seq_t)(N - K + i); /* numerator term */
        res = res * num;                /* multiply then divide exactly */
        res = res / (seq_t)i;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t result = (seq_t)0;

    for (idx_t m = (idx_t)0; ; ++m) {
        idx_t t = (m * (m + (idx_t)1)) / (idx_t)2;   /* triangular number t = m(m+1)/2 */
        if (t > n)
            break;

        seq_t term = binom(n, t);
        if ((n - t) & (idx_t)1)         /* odd exponent => negative sign */
            term = -term;

        result += term;
    }
    return result;
}
