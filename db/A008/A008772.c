/*
 * A008772  Expansion of (1+x^11)/((1-x)*(1-x^2)*(1-x^3)*(1-x^4)).
 * https://oeis.org/A008772
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 168, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A008772:  a(n) = number of partitions of n into 1,2,3,4
    plus the same count for n-11 (when n ≥ 11).
    Generator: (1+x^11)/((1-x)(1-x^2)(1-x^3)(1-x^4))
    Partitions are unordered, computed via the classic coin‑change DP. */

#include <stdlib.h>

/* Static cache of p4(n) = #partitions of n into 1,2,3,4. */
static seq_t *dp = NULL;
static size_t cap = 0;   /* number of elements allocated (dp[0..cap-1]) */

/* Ensure dp[0..n] are computed correctly. */
static void
ensure_cache(idx_t n)
{
    if ((size_t)n + 1 <= cap)        /* already computed up to n */
        return;

    size_t new_cap = (size_t)n + 1;   /* array size needed   */
    seq_t *tmp = (seq_t *)realloc(dp, new_cap * sizeof(seq_t));
    if (tmp == NULL)                 /* allocation failed – leave dp as is */
        return;
    dp   = tmp;
    cap  = new_cap;

    /* Zero the array and set the base case dp[0] = 1. */
    for (idx_t i = 0; i <= n; ++i)
        dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* Unbounded coin‑change DP: outer loop over part sizes. */
    for (int part = 1; part <= 4; ++part)
        for (idx_t i = part; i <= n; ++i)
            dp[i] += dp[i - part];
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    seq_t res = dp[n];
    if (n >= 11)
        res += dp[n - 11];
    return res;
}
