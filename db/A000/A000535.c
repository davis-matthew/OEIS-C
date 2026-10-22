/*
 * A000535  Card matching: coefficients B[n,2] of t^2 in the reduced hit
 *          polynomial A[n,n,n](t).
 * https://oeis.org/A000535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*binomial(n,2)*Sum_{k=0..n-2}
 *            binomial(n,k+2)*binomial(n,k)*binomial(n-2,k) +
 *            3*n^2*Sum_{k=0..n-2}
 *            binomial(n,k+1)*binomial(n-1,k+1)*binomial(n-1,k), rewritten
 *            using binomial(n-2,k) = binomial(n,k+2)*(k+1)(k+2)/(n(n-1)),
 *            binomial(n-1,k+1) = binomial(n,k+1)*(n-k-1)/n, binomial(n-1,k) =
 *            bin...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 2) return 0;

    seq_t c0 = 1;                          /* binomial(n,k)   */
    seq_t c1 = (seq_t)n;                   /* binomial(n,k+1) */
    seq_t c2 = (seq_t)(n * (n - 1) / 2);   /* binomial(n,k+2) */
    seq_t s = 0;

    for (idx_t k = 0; k + 2 <= n; k++) {
        seq_t t1 = c2 * c2;
        t1 = t1 * c0;
        t1 = t1 * (seq_t)((k + 1) * (k + 2) / 2);

        seq_t t2 = c1 * c1;
        t2 = t2 * c1;
        t2 = t2 * (seq_t)((k + 1) * (n - k - 1));

        s += t1;
        s += t2;

        /* advance binomials: binomial(n,k+3) = binomial(n,k+2)*(n-k-2)/(k+3) */
        seq_t nx = c2 * (seq_t)(n - k - 2);
        nx = nx / (seq_t)(k + 3);
        c0 = c1;
        c1 = c2;
        c2 = nx;
    }

    return s * 3;
}
