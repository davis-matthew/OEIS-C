/*
 * A182390  a(0)=0, a(n) = (a(n-1) * n) XOR n.
 * https://oeis.org/A182390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 0, a(n) = (a(n-1) * n) XOR n (bitwise exclusive-OR)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* External integer types definition is provided by the build system. */
static seq_t *cache = NULL;      /* dynamically grown cache of values */
static size_t cache_cap = 0;     /* capacity of the cache array   */
static size_t cache_len = 0;     /* number of terms stored (index+1) */

/* Ensure the cache can hold at least (need+1) terms.  Reallocate if necessary. */
static void
ensure_cache_capacity(idx_t need)
{
    if (cache_cap > need) return;          /* enough space already */
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap <= need) new_cap <<= 1;  /* grow until large enough */

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache) {                       /* out of memory */
        /* In a real project you would handle this properly.
           For the purposes of this sequence generator we abort. */
        exit(1);
    }

    if (cache) {
        memcpy(new_cache, cache, cache_len * sizeof(seq_t));
        free(cache);
    }

    cache = new_cache;
    cache_cap = new_cap;
}

/* Compute a(n) using memoization.  Calls may arrive in any order. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Expand cache to include index n if necessary. */
    if (n >= cache_len) {
        ensure_cache_capacity(n);
        /* Compute missing terms sequentially from cache_len to n inclusive. */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t prev = (i == 0) ? 0 : cache[i - 1];
            cache[i] = (prev * (seq_t)i) ^ (seq_t)i;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
