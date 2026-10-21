/*
 * A223626  Number of 2Xn 0..1 arrays with row sums nondecreasing and column
 *          sums unimodal.
 * https://oeis.org/A223626
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * Let f(0,n) = 1 + n*(n+1)/2 * Let f(u,n) = (n-u+1) +
 *            (u+1)*C(n-u+1, 2) for 1 ≤ u ≤ n * Let A(0) = 1 * Let A(u) =
 *            2^(u-1) for u odd * Let A(u) = (2^u + C(u,u/2)) / 2 for u even *
 *            Then a(n) = Σ_{u=0..n} f(u,n) * A(u)
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 26, u64 n <= 57, u128 n <= 120,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t u, kidx;
    seq_t ans = 0;
    seq_t pow2 = 1; /* 2^u, starts with u = 0 */

    /* iterate through u = 0 .. n */
    for (u = 0; u <= n; ++u)
    {
        /* compute A(u) */
        seq_t A;
        if (u == 0)
            A = 1;
        else
        {
            if (u & 1)   /* u odd */
            {
                A = pow2 / 2;          /* 2^(u-1) */
            }
            else          /* u even */
            {
                /* C(u, u/2) */
                seq_t C = 1;
                idx_t half = u >> 1;
                idx_t i;
                for (i = 1; i <= half; ++i)
                {
                    C = (C * (seq_t)(u - i + 1)) / (seq_t)i;
                }
                A = (pow2 + C) / 2;
            }
        }

        /* compute f(u,n) */
        seq_t fu;
        if (u == 0)
        {
            seq_t nseq = (seq_t)n;
            fu = 1 + (nseq * (nseq + 1)) / 2;
        }
        else
        {
            kidx = n - u + 1;       /* k = n - u + 1 > 0 */
            seq_t k = (seq_t)kidx;
            seq_t binom_k2 = (k * (k - 1)) / 2;
            fu = k + (seq_t)(u + 1) * binom_k2;
        }

        ans += fu * A;

        /* update pow2 for next u */
        if (u < n)
            pow2 = pow2 * 2;
    }

    return ans;
}
