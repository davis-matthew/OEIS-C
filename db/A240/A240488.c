/*
 * A240488  Number of partitions of n containing m(3) as a part, where m
 *          denotes multiplicity.
 * https://oeis.org/A240488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=1}^{⌊n/3⌋} p_{≠3}(n-4k) for k≠3, a(n) += p_{≠3}(n-9)
 *            for k=3, where p_{≠3}(m) counts partitions of m using parts ≠ 3
 * Range:     u8 n <= 20, u16 n <= 47, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *dp = NULL;        /* partitions excluding part 3 */
static idx_t dp_len = 0;       /* allocated length (dp_len = n+1) */

/* Recompute dp up to at least n if needed */
static void ensure_dp(idx_t n) {
    if (dp_len <= n) {
        seq_t *new_dp = (seq_t*)calloc((size_t)(n + 1), sizeof(seq_t));
        if (!new_dp) return;          /* out of memory – silently abort */
        new_dp[0] = (seq_t)1;

        for (idx_t part = 1; part <= n; ++part) {
            if (part == 3) continue;  /* exclude part 3 */
            for (idx_t sum = part; sum <= n; ++sum) {
                new_dp[sum] += new_dp[sum - part];
            }
        }

        free(dp);
        dp = new_dp;
        dp_len = n + 1;
    }
}

seq_t a(idx_t n) {
    ensure_dp(n);

    seq_t ans = 0;
    idx_t maxK = n / 3;
    for (idx_t k = 1; k <= maxK; ++k) {
        if (k == 3) {
            if (n >= (idx_t)9) {
                ans += dp[n - 9];
            }
        } else {
            idx_t sub = 4 * k;
            if (n >= sub) {
                ans += dp[n - sub];
            }
        }
    }
    return ans;
}
