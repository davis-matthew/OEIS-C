/*
 * A170838  G.f.: Product_{k>=0} (1 + x^(2^k-1) + 3x^(2^k)).
 * https://oeis.org/A170838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A170838:
   G.f.: Π_{k≥0} (1 + x^(2^k-1) + 3x^(2^k))
   Computation by in‑place dynamic programming:
   * Each binary exponent 2^k contributes either exponent 0 (implicit),
     exponent 2^k-1 (coeff 1) or exponent 2^k (coeff 3).
   * The case k = 0 is special: 2^0-1 = 0, so the factor is 2 + 3x.
   * Subsequent factors are incorporated by shifting the existing array
     and adding the appropriate contributions.
   This yields a(n) for all n ≥ 0.  */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Allocate DP array of size n+1, zero‑initialized. */
    size_t N = (size_t)n;
    seq_t *dp = (seq_t*)calloc(N + 1, sizeof(seq_t));
    if (dp == NULL) return 0;          /* out of memory → return 0 */

    /* Start with the empty product: a[0] = 1. */
    dp[0] = 1;

    /* ---------- Factor k = 0 : (2 + 3x) ---------- */
    seq_t v = dp[0];
    dp[0] = v * 2;                     /* constant term 2   */
    if (N >= 1) dp[1] = v * 3;          /* term 3x           */

    /* ---------- Factors k ≥ 1 ---------- */
    for (idx_t k = 1; ; ++k) {
        unsigned long long pow2 = 1ULL << k;    /* 2^k, safe for k ≤ 63 */
        idx_t shift1 = (idx_t)(pow2 - 1);        /* 2^k-1  */
        idx_t shift2 = (idx_t)pow2;             /* 2^k    */
        if (shift1 > N && shift2 > N) break;    /* no further effect */

        /* Process in reverse to avoid double counting */
        for (idx_t i = N; ; --i) {
            seq_t val = dp[i];
            if (val) {
                if (i + shift1 <= N)
                    dp[i + shift1] += val;       /* coeff 1 */
                if (i + shift2 <= N)
                    dp[i + shift2] += val * 3;   /* coeff 3 */
            }
            if (i == 0) break;
        }
    }

    seq_t result = dp[N];
    free(dp);
    return result;
}
