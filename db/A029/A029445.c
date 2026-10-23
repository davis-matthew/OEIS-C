/*
 * A029445  Expansion of 1/((1-x^8)(1-x^10)(1-x^11)(1-x^12)).
 * https://oeis.org/A029445
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 8,10,11,12
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Partition problem: compute the coefficient of x^n in
 *       1/((1-x^8)(1-x^10)(1-x^11)(1-x^12))
 * This is a classic unbounded coin‑change problem.
 * We use a memoised dynamic programming array that is expanded
 * as necessary; each extension recomputes the entire table
 * from 0 up to the requested index.
 */

#include <stdlib.h>
#include <string.h>

static seq_t *dp = NULL;      /* dynamic array holding the table   */
static idx_t  dp_max = 0;     /* largest index currently computed */

/* Ensure dp[n] is computed for all 0 ≤ i ≤ n.   */
static void ensure_dp_size(idx_t n)
{
    /* Allocate (or grow) the buffer.  */
    if (dp == NULL) {
        size_t sz = (size_t)(n + 1);
        dp = (seq_t *)calloc(sz, sizeof(seq_t));
        if (!dp) exit(1);                 /* allocation failure */
        dp[0] = 1;
        dp_max = 0;
    } else if (n > dp_max) {
        size_t old_sz = (size_t)(dp_max + 1);
        size_t new_sz = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(dp, new_sz * sizeof(seq_t));
        if (!tmp) exit(1);                 /* allocation failure */
        /* Zero the newly added portion.  */
        memset(tmp + old_sz, 0, (new_sz - old_sz) * sizeof(seq_t));
        dp = tmp;
    }

    /* Recompute the whole table up to n.  */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i) dp[i] = 0;

    const idx_t coins[4] = {8, 10, 11, 12};
    for (idx_t c = 0; c < 4; ++c) {
        idx_t coin = coins[c];
        if (coin > n) continue;
        for (idx_t i = coin; i <= n; ++i) {
            dp[i] += dp[i - coin];
        }
    }
    dp_max = n;
}

seq_t a(idx_t n)
{
    ensure_dp_size(n);
    return dp[n];
}
