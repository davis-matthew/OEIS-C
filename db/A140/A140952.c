/*
 * A140952  Expansion of 1/((1-x^2)*(1-x^3)*(1-x^5)*(1-x^7)*(1-x^11)).
 * https://oeis.org/A140952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into the primes 2,3,5,7,11
 *            (equivalently, the coefficients of
 *            1/((1‑x²)(1‑x³)(1‑x⁵)(1‑x⁷)(1‑x¹¹))
 * Range:     u8 n <= 47, u16 n <= 231, u32 n <= 3914, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* DP array keeps the partition counts up to the largest index seen so far. */
static seq_t *dp   = NULL;         /* dp[i] = a(i) for i < dp_size */
static size_t dp_size = 0;         /* number of entries stored */

/* The five allowed prime parts. */
static const idx_t primes[5] = { 2, 3, 5, 7, 11 };

/* Expand the DP table to include at least index n (inclusive).  The
   DP entry for each i is computed using the standard unbounded coin
   change outer‑coin loop – this guarantees that partitions are counted
   exactly once. */
static void ensure_size(idx_t n)
{
    size_t new_size = (size_t)n + 1;
    if (dp_size >= new_size) return;      /* already large enough */

    /* Allocate (or grow) the array. */
    seq_t *tmp = (seq_t *)realloc(dp, new_size * sizeof(seq_t));
    if (!tmp) exit(1);
    dp = tmp;

    /* Initialise new entries – the array may have been partially
       filled before; the following overwrites the whole range. */
    for (size_t i = 0; i < new_size; ++i) dp[i] = 0;
    dp[0] = 1;   /* there is exactly one way to partition 0 */

    /* Unbounded coin change: outer loop over coin types, inner loop
       over values in ascending order to allow unlimited use of each
       coin. */
    for (int pi = 0; pi < 5; ++pi) {
        idx_t coin = primes[pi];
        if (coin >= new_size) continue;
        size_t c = (size_t)coin;
        for (size_t v = c; v < new_size; ++v) {
            dp[v] += dp[v - c];
        }
    }

    dp_size = new_size;
}

/* Public function required by the build system. */
seq_t a(idx_t n)
{
    ensure_size(n);
    return dp[(size_t)n];
}
