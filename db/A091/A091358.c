/*
 * A091358  Number of planar partitions of n with exactly 4 rows.
 * https://oeis.org/A091358
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A002799(n) - A000991(n)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* compute f_r(n) = coefficient of x^n in ∏_{k≥1} (1 – x^k)^(-min(r,k)) */
static seq_t compute_f(idx_t n, idx_t r)
{
    /* allocate array for DP: dp[j] holds current coefficient of x^j */
    size_t sz = (size_t)(n + 1);
    seq_t *dp = seq_alloc_new_array(sz);
    /* clear array */
    for (size_t i = 0; i < sz; ++i)
        dp[i] = 0;
    dp[0] = 1;

    for (idx_t k = 1; k <= n; ++k)
    {
        idx_t e = (k <= r) ? k : r;  /* exponent = min(r, k) */
        for (idx_t t = 0; t < e; ++t)
        {
            for (idx_t j = k; j <= n; ++j)
            {
                dp[j] += dp[j - k];
            }
        }
    }

    seq_t res = dp[n];
    seq_free_array(dp);
    return res;
}

seq_t a(idx_t n)
{
    /* For n < 4 the offset is not defined; we simply return 0 */
    if (n < 4)
        return 0;

    /* compute A002799(n) and A000991(n) */
    seq_t f4 = compute_f(n, 4);  /* planes partitions with at most 4 rows */
    seq_t f3 = compute_f(n, 3);  /* planes partitions with at most 3 rows */

    return f4 - f3;  /* partitions with exactly 4 rows */
}
