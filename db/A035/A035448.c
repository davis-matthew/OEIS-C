/*
 * A035448  Number of partitions of n into parts 8k+1 or 8k+2.
 * https://oeis.org/A035448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   number of partitions of n into parts 8k+1 or 8k+2
 * Range:     u8 n <= 41, u16 n <= 123, u32 n <= 406, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 76 terms through n = 861 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

seq_t a(idx_t n)
{
    /* OEIS offset is 1; return 0 for undefined n==0 */
    if (n == 0) {
        return 0;
    }

    /* static cache of partial results */
    static seq_t *dp = NULL;   /* array of seq_t values */
    static idx_t max_n = 0;    /* largest n computed so far */

    /* If a larger n is requested, rebuild the table up to n */
    if (dp == NULL || n > max_n) {
        free_seq_array(dp);

        dp = alloc_seq_array(n + 1);
        if (dp == NULL) {
            abort(); /* allocation failure */
        }

        /* initialise DP table */
        for (idx_t i = 0; i <= n; ++i) {
            dp[i] = 0;
        }
        dp[0] = 1;          /* one way to make sum 0 */

        /* 1‑D knapsack DP over parts ≡ 1 or 2 (mod 8) */
        for (idx_t part = 1; part <= n; ++part) {
            if ((part & 7ULL) == 1 || (part & 7ULL) == 2) {
                for (idx_t i = part; i <= n; ++i) {
                    dp[i] = dp[i] + dp[i - part];
                }
            }
        }

        max_n = n;
    }

    return dp[n];
}
