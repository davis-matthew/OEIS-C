/*
 * A227618  Expansion of q * f(-q) * f(-q^4) * f(-q^16) * f(-q^6, -q^10) in
 *          powers of q where f() is a Ramanujan theta function.
 * https://oeis.org/A227618
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is the coefficient of q^n in the power series expansion of
 *            G(q) = q * f(-q) * f(-q^4) * f(-q^6, -q^10) (Ramanujan theta
 *            functions). This is equivalent to the Euler transform of the
 *            period‑16 sequence b =
 *            [-1,-1,-1,-2,-1,-2,-1,-2,-1,-2,-1,-2,-1,-1,-1,-4]. Hence G(q) =
 *            q * ∏_{m≥1} (1 - q^m)^{-...
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* offset 1; behaviour for n<1 is irrelevant */

    idx_t N = n - 1;                     /* we look for coefficient of q^(n-1) in ∏ factors */

#ifdef __cplusplus
    std::vector<seq_t> dp(N+1, 0), newdp(N+1, 0);
#else
    seq_t *dp    = (seq_t *)calloc(N+1, sizeof(seq_t));
    seq_t *newdp = (seq_t *)calloc(N+1, sizeof(seq_t));
    if (!dp || !newdp) return 0;        /* allocation failure – should not happen */
#endif

    dp[0] = 1;                           /* start with the empty product */

    /* period‑16 negative exponent sequence */
    const int b[16] = {-1,-1,-1,-2,-1,-2,-1,-2,-1,-2,-1,-2,-1,-1,-1,-4};

    for (idx_t m = 1; m <= N; ++m) {
        int idx = (int)((m - 1) & 15);     /* (m-1) mod 16 */
        int e   = -b[idx];                 /* positive exponent (0,1,2,3,4) */

        /* precompute coefficients of (1 - q^m)^e : 1 + C(e,1)(-1)q^m + … */
        seq_t coef[5];                     /* e ≤ 4 */
        for (int t = 0; t <= e; ++t) {
            seq_t comb = 1;
            for (int i = 1; i <= t; ++i)
                comb = comb * (e - i + 1) / i;   /* integer binomial */
            if (t & 1) comb = -comb;
            coef[t] = comb;
        }

#ifdef __cplusplus
        std::fill(newdp.begin(), newdp.end(), 0);
#else
        memset(newdp, 0, (N+1) * sizeof(seq_t));
#endif

        for (idx_t k = 0; k <= N; ++k) {
            if (dp[k] == 0) continue;
            for (int t = 0; t <= e; ++t) {
                idx_t nk = k + m * t;
                if (nk > N) break;
                newdp[nk] = newdp[nk] + dp[k] * coef[t];
            }
        }

#ifdef __cplusplus
        std::swap(dp, newdp);
#else
        seq_t *tmp = dp; dp = newdp; newdp = tmp;
#endif
    }

    seq_t result = dp[N];  /* coefficient of q^(n-1) */

#ifndef __cplusplus
    free(newdp);
    free(dp);
#endif
    return result;
}
