/*
 * A033552  Number of partitions into Catalan numbers.
 * https://oeis.org/A033552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 250, u32 n <= 250, u64 n <= 250,
 *            u128 n <= 250, big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of partitions of n into Catalan numbers (C1 = 1, C2 = 2, C3 = 5, …) */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* trivial case */
    if (n == 0)
        return (seq_t)1;

    /* Allocate DP array for sums up to n */
    size_t dp_size = (size_t)n + 1;
    seq_t *dp = (seq_t *)malloc(dp_size * sizeof(seq_t));
    if (!dp)
        return (seq_t)0; /* allocation failure – unlikely in this context */

    /* initialize dp[0] = 1, others = 0 */
    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    /* Generate the list of Catalan part sizes <= n */
    size_t part_cap = 16;
    idx_t *parts = (idx_t *)malloc(part_cap * sizeof(idx_t));
    if (!parts) {
        free(dp);
        return (seq_t)0;
    }
    size_t part_cnt = 0;
    for (idx_t k = 1;; ++k) {
        seq_t cat = catalan(k);
        if ((seq_t)n < cat)
            break;
        if (part_cnt == part_cap) {
            part_cap <<= 1;
            idx_t *tmp = (idx_t *)realloc(parts, part_cap * sizeof(idx_t));
            if (!tmp) {
                free(dp);
                free(parts);
                return (seq_t)0;
            }
            parts = tmp;
        }
        parts[part_cnt++] = (idx_t)cat;
    }

    /* Perform the coin‑change DP using the obtained part sizes */
    for (size_t pi = 0; pi < part_cnt; ++pi) {
        idx_t v = parts[pi];
        for (idx_t i = v; i <= n; ++i) {
            dp[i] += dp[i - v];
        }
    }

    seq_t result = dp[n];
    free(dp);
    free(parts);
    return result;
}
