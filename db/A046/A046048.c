/*
 * A046048  Numbers that are the sum of 17 but no fewer nonzero fourth powers.
 * https://oeis.org/A046048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, nice
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers that are the sum of exactly 17 nonzero fourth powers.
   This implementation builds all integers up to 14000, finds the minimal
   number of fourth powers needed for each, and collects those whose
   minimal count equals 17. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <limits.h>
#include "../utils/seq_alloc.h"

seq_t a(idx_t n)
{
    static seq_t *seq = NULL;
    static size_t seq_len = 0;
    static bool built = false;

    if (!built)
    {
        /* Upper bound: the last term of the sequence is 13792. */
        const idx_t max_val = 14000;

        /* Pre‑compute fourth powers <= max_val. */
        size_t coins_cap = 8;
        uint32_t *coins = (uint32_t *)malloc(coins_cap * sizeof(uint32_t));
        size_t coins_cnt = 0;
        for (uint64_t k = 1;; ++k)
        {
            uint64_t p4 = k * k * k * k;
            if (p4 > max_val) break;
            if (coins_cnt == coins_cap)
            {
                coins_cap *= 2;
                coins = (uint32_t *)realloc(coins, coins_cap * sizeof(uint32_t));
            }
            coins[coins_cnt++] = (uint32_t)p4;
        }

        /* DP: dp[i] = minimal number of fourth powers to represent i. */
        uint16_t *dp = (uint16_t *)malloc((size_t)(max_val + 1) * sizeof(uint16_t));
        for (size_t i = 0; i <= (size_t)max_val; ++i)
            dp[i] = UINT16_MAX;
        dp[0] = 0;

        for (size_t i = 1; i <= (size_t)max_val; ++i)
        {
            for (size_t j = 0; j < coins_cnt; ++j)
            {
                uint32_t c = coins[j];
                if (c > i) break;
                uint16_t prev = dp[i - c];
                if (prev != UINT16_MAX && prev + 1 < dp[i])
                    dp[i] = prev + 1;
            }
        }

        /* Count how many integers need exactly 17 fourth powers. */
        size_t count = 0;
        for (size_t i = 1; i <= (size_t)max_val; ++i)
            if (dp[i] == 17) ++count;

        seq_len = count;
        seq = seq_alloc(seq_len);

        /* Store the qualifying integers in ascending order. */
        size_t idx = 0;
        for (size_t i = 1; i <= (size_t)max_val; ++i)
            if (dp[i] == 17)
                seq[idx++] = (seq_t)i;

        free(coins);
        free(dp);
        built = true;
    }

    if (n == 0 || n > seq_len)
        return (seq_t)0;
    return seq[n - 1];
}
