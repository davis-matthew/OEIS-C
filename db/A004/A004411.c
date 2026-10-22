/*
 * A004411  Expansion of ( Sum_{n = -infinity..infinity} x^(n^2) )^(-10).
 * https://oeis.org/A004411
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   G.f. 1/theta_3(x)^10, so a(n) = (-1)^n * b(n), where Sum b(n)
 *            x^n = 1/theta_4(x)^10 = Product_{k>=1} ((1+x^k)/(1-x^k))^10. For
 *            small n, b is built factor by factor (all coefficients stay
 *            nonnegative and <= b(n)). For large n, the code uses the ODE
 *            recurrence n*b(n) = Sum_{k>=1, k^2<=n} 2*(-1)^(k+...
 * Range:     i8 n <= 1, i16 n <= 4, i32 n <= 12, i64 n <= 37, i128 n <= 120,
 *            big any n
 * Helpers:   release, zalloc_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/zalloc_arr.h"
#include "../utils/release.h"

seq_t a(idx_t n) {
    idx_t N = n + 1;
    seq_t *b = zalloc_arr(N);
    seq_t r;
    idx_t k, i, m;
    int t;

    b[0] = 1;
    if (n <= 300) {
        for (k = 1; k <= n; k++) {
            /* multiply by (1 + x^k)^10 */
            for (t = 0; t < 10; t++)
                for (i = n; i >= k; i--)
                    b[i] += b[i - k];
            /* divide by (1 - x^k)^10 */
            for (t = 0; t < 10; t++)
                for (i = k; i <= n; i++)
                    b[i] += b[i - k];
        }
    } else {
        for (m = 1; m <= n; m++) {
            seq_t s = 0;
            for (k = 1; k * k <= m; k++) {
                seq_t term = ((seq_t)m + (seq_t)(9 * k * k)) * b[m - k * k] * 2;
                if (k & 1) s += term;
                else s -= term;
            }
            b[m] = s / (seq_t)m;
        }
    }
    r = b[n];
    release(b, N);
    if (n & 1) r = -r;
    return r;
}
