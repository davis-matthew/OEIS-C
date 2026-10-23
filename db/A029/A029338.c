/*
 * A029338  Expansion of 1/((1-x^4)*(1-x^5)*(1-x^8)*(1-x^10)).
 * https://oeis.org/A029338
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Number of partitions of n into parts 4, 5, 8 and 10.
   a(0) = 1; a(n) = ∑_{c∈{4,5,8,10}} a(n−c)   (with a(k)=0 for k<0)  */
#include <stdlib.h>

/* Static state for a simple memoised DP table up to the largest
   n ever requested.  The table is recomputed from scratch at
   each call, which keeps the implementation simple and correct. */
static seq_t *dp          = NULL;  /* dp[i] = a(i) for 0 ≤ i ≤ current_cap-1 */
static size_t current_cap = 0;    /* capacity of the dp array (≤ max n seen) */

/* Ensure the table has space for at least 'size' elements.   */
static void ensure_capacity(size_t size)
{
    if (dp == NULL) {
        /* first allocation */
        current_cap = size;
        dp = (seq_t *)malloc(current_cap * sizeof(seq_t));
        if (dp == NULL) abort(); /* OOM */
        return;
    }
    if (size <= current_cap) return;

    /* enlarge, keep existing data (we will fill new area with 0) */
    size_t new_cap = current_cap;
    while (new_cap < size) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
    if (tmp == NULL) abort(); /* OOM */
    /* initialise added slots to zero */
    for (size_t i = current_cap; i < new_cap; ++i) tmp[i] = (seq_t)0;
    dp = tmp;
    current_cap = new_cap;
}

/* Recompute the dp table up to n inclusive.  The recurrence uses
   the classical unbounded‑coin-change DP algorithm.  */
static void recompute(idx_t n)
{
    /* Make sure we have enough space. */
    ensure_capacity((size_t)n + 1);

    /* Initialise all entries to zero. */
    for (size_t i = 0; i <= (size_t)n; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;  /* base case */

    /* Coin values. */
    static const idx_t coins[4] = {4, 5, 8, 10};

    /* For each coin, update the DP table. */
    for (int c = 0; c < 4; ++c) {
        idx_t coin = coins[c];
        for (idx_t i = coin; i <= n; ++i) {
            /* dp[i] += dp[i-coin];  all operations in seq_t */
            dp[(size_t)i] = (seq_t)((seq_t)dp[(size_t)i] + dp[(size_t)(i - coin)]);
        }
    }
}

/* Public function required by the interface.  */
seq_t a(idx_t n)
{
    /* Compute up to n (rebuilding if necessary). */
    recompute(n);
    return dp[(size_t)n];
}
