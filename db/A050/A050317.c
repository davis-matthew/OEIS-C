/*
 * A050317  Number of ways n is a product of partition numbers > 1.
 * https://oeis.org/A050317
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write n as an unordered product of *
 *            partition numbers > 1
 * Range:     u8 n <= 103, u16 n <= 103, u32 n <= 103, u64 n <= 103,
 *            u128 n <= 103, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <vector>
#endif

/* external linkage as required */
seq_t a(idx_t n)
{
    /* static data used for memoisation */
    static idx_t dp_cap = 0;
#ifdef __cplusplus
    static std::vector<seq_t> dp;
    static std::vector<uint64_t> part;
#else
    static seq_t *dp = NULL;
    static uint64_t *part = NULL;
#endif

    /* values below the offset are irrelevant – return 0 */
    if (n < 2)
        return (seq_t)0;

    /* expand storage if needed */
    if (n + 1 > dp_cap) {
        idx_t new_cap = n + 1;
#ifdef __cplusplus
        dp.assign(new_cap, (seq_t)0);
        part.assign(new_cap, (uint64_t)0);
#else
        seq_t *new_dp = (seq_t*)calloc(new_cap, sizeof(seq_t));
        uint64_t *new_part = (uint64_t*)malloc(new_cap * sizeof(uint64_t));
        free(dp);
        free(part);
        dp = new_dp;
        part = new_part;
#endif
        dp_cap = new_cap;

        /* ----- compute partition numbers up to n ----- */
        part[0] = 1;
        for (idx_t i = 1; i <= n; ++i) {
            int64_t sum = 0;
            for (idx_t k = 1;; ++k) {
                idx_t pent1 = k * (3 * k - 1) / 2;
                if (pent1 > i) break;
                int sign = (k & 1) ? 1 : -1;
                sum += sign * (int64_t)part[i - pent1];
                idx_t pent2 = k * (3 * k + 1) / 2;
                if (pent2 > i) continue;
                sum += sign * (int64_t)part[i - pent2];
            }
            part[i] = (uint64_t)sum;
        }

        /* ----- dynamic programme for multiplicative partitions ----- */
        dp[1] = 1;
        for (idx_t k = 2; k <= n; ++k) {
            uint64_t f = part[k];
            if (f < 2 || f > n)
                continue;
            for (idx_t i = f; i <= n; i += f)
                dp[i] += dp[i / f];
        }
    }

    return dp[n];
}
