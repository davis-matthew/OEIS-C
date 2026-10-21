/*
 * A235773  Number of compositions of n into distinct powers of 3 and doubled
 *          powers of 3.
 * https://oeis.org/A235773
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of compositions of n into distinct powers of 3 *
 *            and doubled powers of 3. The order of parts matters. The value *
 *            is sum over all subsets T⊆{3^k,2·3^k} with sum(T)=n of |T|!
 * Range:     u8 n <= 38, u16 n <= 200, u32 n <= 1820, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#ifdef __cplusplus
#include <algorithm>
using namespace std;
#endif

seq_t a(idx_t n) {
    /* Special case n = 0: only the empty composition. */
    if (n == 0) return (seq_t)1;

    /* Gather all allowed parts ≤ n. */
    idx_t parts[32];
    size_t m = 0;
    idx_t power = 1;
    while (power <= n) {
        parts[m++] = power;               /* 3^k  */
        idx_t doublep = power << 1;       /* 2·3^k */
        if (doublep <= n) parts[m++] = doublep;
        if (power > n / 3) break;         /* next power would overflow */
        power *= 3;
    }

    /* Pre‑compute factorials up to m. */
    seq_t fact[32];
    fact[0] = (seq_t)1;
    for (size_t i = 1; i <= m; ++i) fact[i] = fact[i-1] * (seq_t)i;

    /* DP array: dp[s][k] = number of subsets summing to s with k elements. */
    const size_t stride = m + 1;
    const size_t cap = (size_t)(n + 1) * stride;
    seq_t *dp;
#ifdef __cplusplus
    dp = new seq_t[cap];
    std::fill(dp, dp + cap, seq_t(0));
#else
    dp = (seq_t *)calloc(cap, sizeof(seq_t));
#endif
    if (!dp) { /* fallback: return 0 on allocation failure */
        return (seq_t)0;
    }
    dp[0] = (seq_t)1;   /* empty subset for sum 0 */

    /* Transition: add each part one by one. */
    for (size_t idx = 0; idx < m; ++idx) {
        idx_t val = parts[idx];
        for (idx_t s = n; s >= val; --s) {
            for (size_t k = 1; k <= idx + 1; ++k) {
                seq_t prev = dp[((size_t)(s - val)) * stride + (k - 1)];
                if (prev) dp[(size_t)s * stride + k] += prev;
            }
            if (s == val) break;
        }
    }

    /* Compute the answer: sum over all k of dp[n][k] * k!. */
    seq_t result = (seq_t)0;
    size_t base = (size_t)n * stride;
    for (size_t k = 0; k <= m; ++k) {
        seq_t ways = dp[base + k];
        if (ways) result += ways * fact[k];
    }

#ifdef __cplusplus
    delete[] dp;
#else
    free(dp);
#endif
    return result;
}
