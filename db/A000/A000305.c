/*
 * A000305  Number of certain rooted planar maps.
 * https://oeis.org/A000305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{m=1..n} Fibonacci(m+1) * (m/n) * binomial(3n-m-1,
 *            n-m), i.e. A(x) = (u+u^2)/(1-u-u^2) where u = x/(1-u)^2 (u = x +
 *            2x^2 + 7x^3 + ...)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 26, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 60) {
        /* division-free power-series computation (exact modulo 2^w for fixed widths) */
        seq_t u[61], u2[61], u3[61], s[61], A[61];
        long long N = (long long)n, k, i;
        for (k = 0; k <= N; k++) {
            u[k] = 0; u2[k] = 0; u3[k] = 0; s[k] = 0; A[k] = 0;
        }
        for (k = 1; k <= N; k++) {
            seq_t q2 = 0, q3 = 0;
            for (i = 1; i <= k - 1; i++) q2 += u[i] * u[k - i];
            for (i = 1; i <= k - 2; i++) q3 += u[i] * u2[k - i];
            u2[k] = q2;
            u3[k] = q3;
            u[k] = (seq_t)(k == 1 ? 1 : 0) + 2 * q2 - q3;
            s[k] = u[k] + u2[k];
            {
                seq_t acc = s[k];
                for (i = 1; i <= k - 1; i++) acc += s[i] * A[k - i];
                A[k] = acc;
            }
        }
        return A[N];
    } else {
        /* exact summation (only reached by arbitrary-precision builds in practice) */
        seq_t b = 1, t, sum = 0, f = 1, fprev = 1, tmp;
        idx_t i, m;
        for (i = 1; i <= n - 1; i++)
            b = b * (seq_t)(2 * n - 1 + i) / (seq_t)i;   /* binomial(3n-2, n-1) */
        t = b / (seq_t)n;                                  /* t_1 = [x^n] u */
        for (m = 1; m <= n; m++) {
            sum += f * t;                                  /* f = Fibonacci(m+1) */
            if (m < n)
                t = t * (seq_t)(m + 1) * (seq_t)(n - m)
                    / ((seq_t)m * (seq_t)(3 * n - m - 1));
            tmp = f + fprev;
            fprev = f;
            f = tmp;
        }
        return sum;
    }
}
