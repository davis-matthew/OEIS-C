/*
 * A029147  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^5)*(1-x^10)).
 * https://oeis.org/A029147
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into parts 2, 3, 5, and 10
 * Range:     u8 n <= 67, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is computed by dynamic programming: */
/*   dp[0] = 1; for each part p in {2,3,5,10}:   */
/*   for i = p .. n: dp[i] += dp[i - p] */
/* The function caches the largest n that has been computed so far
 * and recomputes the entire table if a larger n is requested.
 * All calculations are performed in seq_t to avoid overflows. */

#include <stdlib.h>

/* static variables holding the cache */
static seq_t *dp   = NULL;      /* array of computed values           */
static idx_t  cap  = 0;         /* current capacity of dp array       */
static idx_t  max  = 0;         /* largest index that has been computed */

/* parts allowed in the partitions */
static const idx_t parts[4] = {2, 3, 5, 10};

/* ensure the dp array has space for indices 0 .. n */
static void ensure_capacity(idx_t n)
{
    if (n < cap) return;                      /* already sufficient */

    /* grow to at least n+1, doubling each step */
    idx_t new_cap = cap ?: 1;                  /* start at 1 if not yet allocated */
    while (new_cap <= n) new_cap <<= 1;

    seq_t *new_dp = (seq_t *)realloc(dp, sizeof(seq_t) * new_cap);
    if (!new_dp) {                 /* allocation failed – terminate program */
        abort();
    }
    dp = new_dp;
    cap = new_cap;
}

/* compute dp table up to n (inclusive) starting from scratch */
static void recompute_until(idx_t n)
{
    /* make sure we have room */
    ensure_capacity(n);

    /* initialise all values to zero: dp[0] will be set to 1 below      */
    for (idx_t i = 0; i <= n; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* run the classic coin‑change DP algorithm */
    for (size_t pi = 0; pi < 4; ++pi) {
        idx_t p = parts[pi];
        for (idx_t i = p; i <= n; ++i) {
            dp[i] += dp[i - p];
        }
    }
    max = n;
}

seq_t a(idx_t n)
{
    /* initialise cache on first call */
    if (!dp) {
        ensure_capacity(n);
        dp[0] = (seq_t)1;             /* dp[0] always 1 */
        max = 0;
    }

    /* if we need larger index, recompute all values up to n */
    if (n > max) {
        recompute_until(n);
    }

    /* value is already known for n <= max */
    return dp[n];
}
