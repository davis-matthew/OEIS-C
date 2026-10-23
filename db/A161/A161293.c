/*
 * A161293  Number of partitions of n into numbers not divisible by 4 where
 *          every part appears at least 2 times.
 * https://oeis.org/A161293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of x^n in ∏_{k≥1, 4∤k} (1 - x^k +
 *            x^{2k}) / (1 - x^k) i.e. the number of partitions of n into
 *            parts not divisible by 4 where each part appears 0 or at least 2
 *            times. The function uses a dynamic‑programming algorithm with
 *            prefix‑sums that works for any integer type seq_t (i...
 * Range:     u8 n <= 31, u16 n <= 82, u32 n <= 243, u64 n <= 802,
 *            u128 n <= 802, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    size_t N = (size_t)n;
    seq_t *dp = NULL;
    seq_t *newdp = NULL;
    seq_t *pref = NULL;

#ifdef __cplusplus
    dp = new seq_t[N + 1];
    newdp = new seq_t[N + 1];
    pref = new seq_t[N + 1];
#else
    dp = (seq_t*)calloc(N + 1, sizeof(seq_t));
    newdp = (seq_t*)calloc(N + 1, sizeof(seq_t));
    pref = (seq_t*)calloc(N + 1, sizeof(seq_t));
#endif
    if (!dp || !newdp || !pref) {
#ifdef __cplusplus
        delete[] dp;
        delete[] newdp;
        delete[] pref;
#else
        free(dp);
        free(newdp);
        free(pref);
#endif
        return (seq_t)0;
    }

    dp[0] = (seq_t)1;  /* empty partition */

    for (size_t k = 1; k <= N; ++k) {
        if (k % 4 == 0) continue;          /* exclude multiples of 4 */

        /* Prefix sums: pref[s] = Σ_{t>=0} dp[s - t*k] */
        pref[0] = dp[0];
        for (size_t s = 1; s <= N; ++s) {
            pref[s] = dp[s];
            if (s >= k) pref[s] += pref[s - k];
        }

        /* Compute newdp without the m = 1 term */
        for (size_t s = 0; s <= N; ++s) {
            newdp[s] = pref[s];
            if (s >= k) newdp[s] -= dp[s - k];
        }

        /* Swap dp and newdp for the next iteration */
        seq_t *tmp = dp; dp = newdp; newdp = tmp;
    }

    seq_t result = dp[N];

#ifdef __cplusplus
    delete[] dp;
    delete[] newdp;
    delete[] pref;
#else
    free(dp);
    free(newdp);
    free(pref);
#endif
    return result;
}
