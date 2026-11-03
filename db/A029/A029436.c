/*
 * A029436  Expansion of 1/((1-x^7)(1-x^8)(1-x^10)(1-x^12)).
 * https://oeis.org/A029436
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of solutions to 7a+8b+10c+12d = n, i.e. the
 *            coefficient of 1/((1-x^7)(1-x^8)(1-x^10)(1-x^12))
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Validated: partial, pass, 73 terms through n = 72 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The sequence is a classic coin‑change counting problem with the four
    coin denominations {7,8,10,12}.  We compute the number of ways to
    form n as an unordered sum of these coins using a standard dynamic
    programming routine. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
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
    /* Allocate an array big enough to hold the DP table for 0 … n.  */
    seq_t *dp = alloc_seq_array((size_t)n + 1);
    if (!dp)
        return (seq_t)0;                     /* defence against OOM */

    /* Initialise the table.  */
    for (size_t i = 1; i <= (size_t)n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;                       /* one way to make zero */

    /* The four coin values.  */
    const idx_t coins[4] = {7, 8, 10, 12};

    /* Standard unbounded‑knapsack coin‑change DP.  */
    for (int c = 0; c < 4; ++c) {
        idx_t coin = coins[c];
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] += dp[i - coin];
        }
    }

    seq_t result = dp[(size_t)n];
    free_seq_array(dp);
    return result;
}
