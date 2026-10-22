/*
 * A000489  Card matching: Coefficients B[n,3] of t^3 in the reduced hit
 *          polynomial A[n,n,n](t).
 * https://oeis.org/A000489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*binomial(n,3)*Sum_{k=0..n-3}
 *            binomial(n,k+3)*binomial(n,k)*binomial(n-3,k) +
 *            6*n*binomial(n,2)*Sum_{k=0..n-3}
 *            binomial(n,k+1)*binomial(n-1,k+2)*binomial(n-2,k) +
 *            n^3*Sum_{k=0..n-1} binomial(n-1,k)^3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 0;
    seq_t N = (seq_t)n;

    if (n >= 3) {
        /* first sum */
        seq_t b1 = N * (N - 1) * (N - 2) / 6;   /* C(n, k+3) */
        seq_t b2 = 1;                           /* C(n, k)   */
        seq_t b3 = 1;                           /* C(n-3, k) */
        seq_t sum1 = 0;
        seq_t cn3 = b1;
        idx_t k;
        for (k = 0; k <= n - 3; k++) {
            sum1 += b1 * b2 * b3;
            b1 = b1 * (seq_t)(n - k - 3) / (seq_t)(k + 4);
            b2 = b2 * (seq_t)(n - k) / (seq_t)(k + 1);
            b3 = b3 * (seq_t)(n - 3 - k) / (seq_t)(k + 1);
        }
        result += 3 * cn3 * sum1;

        /* second sum */
        {
            seq_t c1 = N;                            /* C(n, k+1)   */
            seq_t c2 = (N - 1) * (N - 2) / 2;        /* C(n-1, k+2) */
            seq_t c3 = 1;                            /* C(n-2, k)   */
            seq_t sum2 = 0;
            seq_t cn2 = N * (N - 1) / 2;
            for (k = 0; k <= n - 3; k++) {
                sum2 += c1 * c2 * c3;
                c1 = c1 * (seq_t)(n - k - 1) / (seq_t)(k + 2);
                c2 = c2 * (seq_t)(n - k - 3) / (seq_t)(k + 3);
                c3 = c3 * (seq_t)(n - 2 - k) / (seq_t)(k + 1);
            }
            result += 6 * N * cn2 * sum2;
        }
    }

    {
        seq_t d = 1;    /* C(n-1, k) */
        seq_t sum3 = 0;
        idx_t k;
        for (k = 0; k < n; k++) {
            sum3 += d * d * d;
            d = d * (seq_t)(n - 1 - k) / (seq_t)(k + 1);
        }
        result += N * N * N * sum3;
    }

    return result;
}
