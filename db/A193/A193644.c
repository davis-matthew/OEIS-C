/*
 * A193644  Number of arrays of -4..4 integers x(1..n) with every x(i) in a
 *          subsequence of length 1 or 2 with sum zero.
 * https://oeis.org/A193644
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 6*a(n-2) + a(n-3); Initial values: a(1)=1,
 *            a(2)=9, a(3)=25
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 34, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/* Dynamic memoisation cache. */
static seq_t *cache   = NULL;
static bool  *cached  = NULL;
static idx_t cache_size = 0;

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n + 1 <= cache_size) return;          /* already large enough */

    size_t new_size = (size_t)(n + 1);

    /* Allocate new storage. */
    seq_t  *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    bool   *new_cached = (bool  *)malloc(new_size * sizeof(bool));
    if (!new_cache || !new_cached) abort();

    /* Copy old data if present. */
    if (cache_size > 0) {
        memcpy(new_cache,   cache,   cache_size * sizeof(seq_t));
        memcpy(new_cached,  cached,  cache_size * sizeof(bool));
        free(cache);
        free(cached);
    }

    /* Mark new slots as not computed. */
    size_t i;
    for (i = cache_size; i < new_size; ++i) {
        new_cached[i] = false;
    }

    cache      = new_cache;
    cached     = new_cached;
    cache_size = (idx_t)new_size;
}

/* Return the n-th term (n ≥ 1). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0 */

    ensure_cache(n);

    /* Compute missing terms on demand. */
    idx_t i;
    for (i = 1; i <= n; ++i) {
        if (cached[i]) continue;     /* already computed */

        if (i == 1) {
            cache[i] = (seq_t)1;
        } else if (i == 2) {
            cache[i] = (seq_t)9;
        } else if (i == 3) {
            cache[i] = (seq_t)25;
        } else {
            cache[i] = cache[i-1] * (seq_t)2
                     + cache[i-2] * (seq_t)6
                     + cache[i-3];
        }
        cached[i] = true;
    }

    return cache[n];
}
