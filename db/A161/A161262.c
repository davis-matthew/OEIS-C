/*
 * A161262  Number of partitions of n into central polygonal numbers A000124
 *          where every part appears at least 9 times.
 * https://oeis.org/A161262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into central polygonal numbers
 *            (k*(k+1)/2+1) where each part appears at least 9 times.
 *            Computation: dynamic programming over part sizes. For each part
 *            value s, we compute dp2[sum] = dp_old[sum-9*s] + dp2[sum-s] for
 *            sum ≥ 9*s. dp2[sum] counts partitions that use pa...
 * Range:     u8 n <= 109, u16 n <= 322, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 105 terms through n = 966 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0) {
        return 0;
    }

    /* allocate DP arrays */
    size_t sz = (size_t)(n + 1);
    seq_t *dp = mem_new(seq_t, sz);
    seq_t *newdp = mem_new(seq_t, sz);
    seq_t *dp2 = mem_new(seq_t, sz);
    if (!dp || !newdp || !dp2) {
        /* allocation failed; return 0 as safe fallback */
        mem_free(dp); mem_free(newdp); mem_free(dp2);
        return 0;
    }
    dp[0] = 1;                      /* empty partition */

    /* build list of central polygonal numbers ≤ n */
    idx_t *parts = NULL;
    size_t parts_cap = 16;
    size_t parts_cnt = 0;
    parts = (idx_t *)malloc(parts_cap * sizeof(idx_t));
    if (!parts) {
        mem_free(dp); mem_free(newdp); mem_free(dp2);
        return 0;
    }
    for (idx_t k = 0;; ++k) {
        idx_t val = k * (k + 1) / 2 + 1;
        if (val > n) break;
        if (parts_cnt == parts_cap) {
            parts_cap <<= 1;
            parts = (idx_t *)realloc(parts, parts_cap * sizeof(idx_t));
            if (!parts) {
                mem_free(dp); mem_free(newdp); mem_free(dp2);
                return 0;
            }
        }
        parts[parts_cnt++] = val;
    }

    /* process each part value */
    for (size_t pi = 0; pi < parts_cnt; ++pi) {
        idx_t s = parts[pi];

        /* start newdp as copy of dp */
        for (idx_t i = 0; i <= n; ++i) {
            newdp[i] = dp[i];
            dp2[i] = 0;
        }

        /* 9*s must be ≤ n to allow any use of this part */
        if (s <= n / 9) {
            idx_t start = 9 * s;
            for (idx_t sum = start; sum <= n; ++sum) {
                /* dp2[sum] counts partitions that use part s at least 9 times */
                dp2[sum] = dp[sum - start];
                if (sum >= s) {
                    dp2[sum] += dp2[sum - s];
                }
                newdp[sum] = dp[sum] + dp2[sum];
            }
        }

        /* swap dp and newdp for next part */
        seq_t *tmp = dp;
        dp = newdp;
        newdp = tmp;
    }

    seq_t result = dp[n];

    /* cleanup */
    mem_free(dp);
    mem_free(newdp);
    mem_free(dp2);
    free(parts);

    return result;
}
