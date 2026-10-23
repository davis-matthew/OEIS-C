/*
 * A036002  Number of partitions in parts not of the form 25k, 25k+3 or 25k-3.
 *          Also number of partitions with at most 2 parts of size 1 and
 *          differences between parts at distance 11 are greater than 1.
 * https://oeis.org/A036002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes number of partitions of n into parts not congruent to 0, 3, or 22 modulo 25.
   Uses dynamic programming and caches results for repeated calls. */
#include <stdint.h>
#include <stdlib.h>

/* External integer types provided by the build system */
typedef idx_t idx_t;      /* index type (uint64_t) */
typedef seq_t seq_t;      /* sequence value type (uint8/16/32/64/128 bits or arbitrary-precision) */

static seq_t *cache = NULL;   /* cache for computed values, cache[i] = a(i+1) */
static idx_t cache_len = 0;   /* number of entries currently cached */

/* Main function requested by the specifications */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1, but safe guard */

    /* Return cached value if already computed */
    if (n <= cache_len) {
        return cache[n - 1];
    }

    /* Compute values up to new_n = n */
    idx_t new_n = n;
    /* Allocate workspace for dynamic programming */
    seq_t *dp = (seq_t *)malloc((new_n + 1) * sizeof(seq_t));
    if (!dp) return 0;            /* allocation failed – unlikely in this context */

    /* Initial condition: one way to partition 0 */
    dp[0] = 1;
    for (idx_t i = 1; i <= new_n; ++i) dp[i] = 0;

    /* Dynamic programming over allowed part sizes */
    for (idx_t part = 1; part <= new_n; ++part) {
        /* Skip disallowed parts: multiples of 25, 25k+3, 25k+22 */
        if ( (part % 25 == 0) || (part % 25 == 3) || (part % 25 == 22) )
            continue;

        for (idx_t sum = part; sum <= new_n; ++sum) {
            dp[sum] += dp[sum - part];
        }
    }

    /* Resize cache to hold up to new_n entries */
    seq_t *new_cache = (seq_t *)realloc(cache, new_n * sizeof(seq_t));
    if (!new_cache) {
        free(dp);
        return 0;   /* out of memory; unlikely */
    }
    cache = new_cache;
    cache_len = new_n;

    /* Store computed values into the cache */
    for (idx_t i = 0; i < new_n; ++i) {
        cache[i] = dp[i + 1];   /* dp[0] corresponds to n=0, so offset by +1 */
    }

    /* Result for current request */
    seq_t result = cache[n - 1];

    free(dp);
    return result;
}
