/*
 * A218569  Number of partitions p of n such that max(p)-min(p) = 6.
 * https://oeis.org/A218569
 *
 * Offset:    8    (a(8) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n such that max(p)-min(p) = 6 .
 *            The algorithm pre‑computes all values up to the maximum
 *            requested index in one sweep. For each possible minimum part m
 *            the number of partitions of the remaining sum R = n - 2*m - 6
 *            into parts between m and m+6 is counted with a classi...
 * Range:     u8 n <= 25, u16 n <= 70, u32 n <= 480, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   compute_to
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/compute_to.h"

/* seq_t and idx_t are supplied by the build environment. */

/* Cached values of a(n).  They are allocated lazily and rebuilt only
   when a larger n is requested. */
static seq_t *cache   = NULL;   /* array of a(n) values */
static idx_t cache_len = 0;     /* current size of cache (number of entries) */

/* --------------------------------------------------------------------
   Ensure that the cache contains values up to at least n.
   Rebuild the cache if necessary.
   -------------------------------------------------------------------- */
static void ensure_cache(idx_t n)
{
    if (n < 8) return;                /* values below offset are ignored */

    if (!cache || cache_len <= n) {
        /* Allocate a new cache big enough for the requested n.
           We also grow the cache in chunks to reduce reallocations. */
        idx_t new_len = n + 1;
        /* Allocate new cache array. */
        seq_t *new_cache = (seq_t *)calloc(new_len, sizeof(seq_t));
        if (!new_cache) return;       /* allocation failure */

        /* Compute all values up to n. */
        compute_to(n, new_cache);

        /* Replace old cache if it existed. */
        if (cache) free(cache);
        cache     = new_cache;
        cache_len = new_len;
    }
}

/* --------------------------------------------------------------------
   Public function required by the test harness.
   -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    ensure_cache(n);
    if (cache && n < cache_len) return cache[n];
    /* Undefined for n < 8; return 0. */
    return 0;
}
