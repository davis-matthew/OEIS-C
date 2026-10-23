/*
 * A199016  Number of partitions of n into terms of (1,2)-Ulam sequence, cf.
 *          A002858.
 * https://oeis.org/A199016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into (1,2)-Ulam numbers
 *            (unlimited multiplicity)
 * Range:     u8 n <= 19, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Base cases: a(0) = 1 (empty partition) */
    if (n == 0)
        return (seq_t)1;

    /* Generate all (1,2)-Ulam numbers ≤ n */
    size_t ulam_cap = (size_t)n + 2;                /* enough for the first few numbers */
    uint32_t *ulam = (uint32_t *)malloc(ulam_cap * sizeof *ulam);
    if (!ulam) return (seq_t)1;                      /* out of memory -> fail safe */

    size_t ulam_len = 0;
    ulam[ulam_len++] = 1;                            /* first Ulam number */
    if (n >= 2) {
        ulam[ulam_len++] = 2;                        /* second Ulam number */
    }

    /* Count occurrences of sums of two distinct Ulam numbers
       up to 2*n (since the next Ulam number cannot exceed n). */
    size_t max_sum = (size_t)n * 2;
    uint16_t *cnt = (uint16_t *)calloc(max_sum + 1, sizeof *cnt);
    if (!cnt) { free(ulam); return (seq_t)1; }

    if (ulam_len >= 2) cnt[3] = 1;                    /* 1 + 2 */

    uint32_t candidate = 3;
    while (candidate <= n) {
        if (cnt[candidate] == 1) {                   /* unique sum: it's an Ulam number */
            if (ulam_len == ulam_cap) {               /* grow array */
                ulam_cap *= 2;
                uint32_t *tmp = (uint32_t *)realloc(ulam, ulam_cap * sizeof *ulam);
                if (!tmp) { free(cnt); free(ulam); return (seq_t)1; }
                ulam = tmp;
            }
            ulam[ulam_len++] = candidate;

            /* Update sum counts for new term with all previous Ulam numbers. */
            for (size_t i = 0; i < ulam_len - 1; ++i) {
                uint32_t s = candidate + ulam[i];
                if (s <= max_sum) cnt[s]++;          /* s fits in uint16_t for n<=10000 */
            }
        }
        candidate++;
    }

    /* Dynamic programming for partitions using the Ulam numbers (unlimited usage). */
    seq_t *dp = (seq_t *)calloc((size_t)n + 1, sizeof *dp);
    if (!dp) { free(cnt); free(ulam); return (seq_t)1; }

    dp[0] = (seq_t)1;                                 /* empty partition */

    for (size_t k = 0; k < ulam_len; ++k) {
        uint32_t coin = ulam[k];
        for (idx_t s = (idx_t)coin; s <= n; ++s) {
            dp[s] += dp[s - coin];
        }
    }

    seq_t result = dp[n];

    /* Clean up */
    free(dp);
    free(cnt);
    free(ulam);

    return result;
}
