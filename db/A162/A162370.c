/*
 * A162370  Number of reduced words of length n in the Weyl group D_28.
 * https://oeis.org/A162370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = coefficient of x^n in f(28) * Π_{i=1}^{27} f(2*i), where
 *            f(m) = 1 + x + ... + x^{m-1}
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 44, u128 n <= 756,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute a(n) for the OEIS sequence A162370. */
seq_t a(idx_t n) {
    /* The maximal degree of the polynomial is 756. */
    enum { MAXDEG = 756 };
    static seq_t dp[MAXDEG + 1];
    static seq_t tmp[MAXDEG + 1];
    static int initialized = 0;

    if (!initialized) {
        /* Initialize dp to zero and set dp[0] = 1. */
        for (int i = 0; i <= MAXDEG; ++i)
            dp[i] = (seq_t)0;
        dp[0] = (seq_t)1;

        /* List of factor exponents m: first 28, then 2,4,…,54. */
        const int m_vals[28] = {
            28,
            2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26,
            28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52, 54
        };

        /* Convolve dp with each f(m). */
        for (int idx = 0; idx < 28; ++idx) {
            int m = m_vals[idx];

            /* Zero tmp. */
            for (int i = 0; i <= MAXDEG; ++i)
                tmp[i] = (seq_t)0;

            /* Perform convolution: (dp * f(m)) */
            for (int s = 0; s <= MAXDEG; ++s) {
                seq_t val = dp[s];
                if (val == (seq_t)0)
                    continue;
                for (int t = 0; t < m; ++t) {
                    int ns = s + t;
                    if (ns > MAXDEG)
                        break;
                    tmp[ns] += val;
                }
            }

            /* Copy back to dp. */
            for (int i = 0; i <= MAXDEG; ++i)
                dp[i] = tmp[i];
        }

        initialized = 1;
    }

    if (n > (idx_t)MAXDEG)
        return (seq_t)0;
    return dp[(int)n];
}
