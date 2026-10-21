/*
 * A062684  Number of addition triangles with apex n (version 1).
 * https://oeis.org/A062684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Σ_{k=0}^{⌊log₂n⌋} #{ non‑negative integer solutions to
 *            Σ_{i=0}^k C(k,i) b_i = n – 2^k } (b_i ≥ 0)
 * Range:     u8 n <= 22, u16 n <= 100, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#ifdef __cplusplus
#include <cstddef>
#include <cstdint>
#endif

/* Helper: integer binomial coefficient */
static idx_t binomial(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    idx_t res = 1;
    for (idx_t i = 1; i <= k; ++i)
        res = res * (n - i + 1) / i;
    return res;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 */

    /* determine largest power of two ≤ n (used for loop bounds) */
    idx_t kmax;
    {
        idx_t power = 1;
        idx_t k = 0;
        while (power <= n && power != 0) {
            ++k;
            power <<= 1;
        }
        kmax = k - 1;
    }

    /* allocate a flat array for all DP tables */
    const size_t rows = (size_t)kmax + 1;
    const size_t cols = (size_t)n + 1;
    const size_t total = rows * cols;
    seq_t *dp_all = NULL;

#ifdef __cplusplus
    dp_all = new seq_t[total];
#else
    dp_all = (seq_t *)calloc(total, sizeof(seq_t));
#endif

    if (!dp_all) return (seq_t)0;  /* allocation failed */

    /* compute DP tables for each k */
    for (idx_t k = 0; k <= kmax; ++k) {
        seq_t *dp = dp_all + k * cols;
        dp[0] = (seq_t)1;          /* one way to make sum 0 with no numbers */

        /* weights are binomial(k,i) for i=0..k */
        for (idx_t i = 0; i <= k; ++i) {
            idx_t w = binomial(k, i);
            if (w > n) continue;
            for (idx_t s = w; s <= n; ++s) {
                dp[s] += dp[s - w];
            }
        }
    }

    /* sum contributions for each k */
    seq_t result = (seq_t)0;
    for (idx_t k = 0; k <= kmax; ++k) {
        idx_t pow2 = ((idx_t)1) << k;
        if (pow2 > n) continue;
        idx_t rem = n - pow2;
        seq_t *dp = dp_all + k * cols;
        result += dp[rem];
    }

    /* free DP memory */
#ifdef __cplusplus
    delete[] dp_all;
#else
    free(dp_all);
#endif

    return result;
}
