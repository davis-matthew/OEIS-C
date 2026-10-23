/*
 * A029072  Expansion of 1/((1-x)*(1-x^4)*(1-x^6)*(1-x^11)).
 * https://oeis.org/A029072
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 1, 4, 6 and 11
 *            Computation: dynamic programming – unbounded‑knapsack counting
 *            partitions, where order of parts does not matter
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Static cache for previously allocated workspace; all entries are
   recomputed every call so that results are independent of previous
   calls or order of requests.  */
static seq_t *cache = NULL;   /* array of dp values   */
static size_t cache_len = 0;  /* allocated size       */

/* Ensure that the cache can hold indices 0..n.   */
static void ensure_cache(size_t need)
{
    if (need <= cache_len)
        return;
    size_t new_len = cache_len ? cache_len << 1 : 1;
    while (new_len < need)
        new_len <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)
        abort();                       /* allocation failure   */
    if (cache_len == 0)                 /* first allocation    */
        tmp[0] = 1;                      /* dp[0] = 1           */
    else
        for (size_t i = cache_len; i < new_len; ++i)
            tmp[i] = 0;                  /* zero‑init new part  */
    cache = tmp;
    cache_len = new_len;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    size_t need = (size_t)n + 1;          /* need indices 0..n  */
    ensure_cache(need);

    /* Recompute DP table up to n:  dp[0] = 1, others 0   */
    cache[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        cache[i] = 0;

    /* Unbounded‑knapsack: coin order does not matter,
       so outer loop is over coins, inner over sums. */
    const idx_t coins[4] = {1, 4, 6, 11};
    for (size_t c = 0; c < 4; ++c) {
        idx_t coin = coins[c];
        for (idx_t i = coin; i <= n; ++i)
            cache[i] += cache[i - coin];
    }

    return cache[n];
}
