/*
 * A035375  Number of partitions of n into parts 6k or 6k+1.
 * https://oeis.org/A035375
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 47, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the number of partitions of n into parts of the form 6k or 6k+1.
   Uses a standard unbounded coin‑change DP (order of summands does not matter). */
#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* offset starts at 1 – this case is not used */

    seq_t *dp = alloc_seq_array(n + 1);
    if (!dp)
        return 0;   /* allocation failed */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = 0;
    dp[0] = 1;       /* empty partition */

    for (idx_t k = 0;; ++k)
    {
        idx_t part1 = 6 * k + 1;   /* numbers ≡ 1 (mod 6) */
        idx_t part0 = 6 * k;       /* numbers divisible by 6 */

        if (part1 > n && part0 > n)
            break;

        if (part1 <= n)
            for (idx_t i = part1; i <= n; ++i)
                dp[i] += dp[i - part1];

        if (part0 != 0 && part0 <= n)
            for (idx_t i = part0; i <= n; ++i)
                dp[i] += dp[i - part0];
    }

    seq_t result = dp[n];
    free_seq_array(dp);
    return result;
}
