/*
 * A044044  Catafusenes (see reference for precise definition).
 * https://oeis.org/A044044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of z^n in * G(z) = U(z^2)*V(z) + V(z)/z -
 *            V(z^3) - 1 * U(z) = (1 - 3z - sqrt(1-6z+5z^2)) / (2z) * V(z) =
 *            (1 - z^2 - sqrt(1-6z^2+5z^4)) / (2z(1-z)) * The implementation
 *            expands the series only up to the requested index n. * It uses
 *            only integer arithmetic and the provided type s...
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 31, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Special case: a(0) = 0, handled naturally by the algorithm */
    size_t N = (size_t)n;                 /* index up to N */
    size_t maxAbs = (N + 2) / 2 + 1;      /* number of abs_s terms needed */
    seq_t *abs_s = (seq_t *)malloc((maxAbs + 1) * sizeof(seq_t));
    if (!abs_s) return 0;                 /* allocation fail: return 0 */

    /* abs_s[0] represents |s_0| = 1 (though not used in sums)           */
    abs_s[0] = 1;
    /* base cases */
    abs_s[1] = 3;          /* |s_1| = 3 */
    abs_s[2] = 2;          /* |s_2| = 2 */
    /* higher terms: |s_n| = (sum_{k=1}^{n-1} |s_k|*|s_{n-k}|) / 2
       for n >= 3   (since s_n is negative)                           */
    for (size_t i = 3; i <= maxAbs; ++i) {
        seq_t sum = 0;
        for (size_t k = 1; k < i; ++k) {
            sum += abs_s[k] * abs_s[i - k];
        }
        abs_s[i] = sum / 2;
    }

    /* Compute u terms (coefficients of U(z)):  u_k = |s_{k+1}|/2 for k>=1 */
    size_t maxU = N / 2;                 /* floor(N/2) */
    seq_t *u = NULL;
    if (maxU >= 1) {
        u = (seq_t *)malloc((maxU + 1) * sizeof(seq_t));
        if (!u) { free(abs_s); return 0; }
        for (size_t k = 1; k <= maxU; ++k) {
            u[k] = abs_s[k + 1] / 2;
        }
    }

    /* Compute v terms (coefficients of V(z)).  v[0] = 0 */
    size_t maxV = N + 1;                 /* need v up to index N+1 for V(z)/z term */
    seq_t *v = (seq_t *)malloc((maxV + 1) * sizeof(seq_t));
    if (!v) { free(abs_s); free(u); return 0; }
    v[0] = 0;
    for (size_t k = 1; k <= maxV; ++k) {
        seq_t mk = 0;
        if ((k & 1) != 0) {                /* odd k */
            if (k == 1) {
                mk = 1;
            } else {
                size_t idx = (k + 1) / 2;  /* idx >= 2 */
                mk = abs_s[idx] / 2;
            }
        }
        v[k] = v[k - 1] + mk;
    }

    /* Compute a(N) = c_N + v[N+1] - v[N/3] (when N%3==0) - 1 if N==0 */
    seq_t cN = 0;
    for (size_t p = 1; p <= maxU; ++p) {
        size_t i = 2 * p;
        if (i > N) break;
        cN += u[p] * v[N - i];
    }
    seq_t term2 = v[N + 1];
    seq_t term3 = (N % 3 == 0) ? v[N / 3] : 0;
    seq_t ans = cN + term2;
    if (term3 != 0) ans -= term3;
    if (N == 0) ans -= 1;

    free(abs_s);
    free(u);
    free(v);
    return ans;
}
