/*
 * A050026  a(n) = a(n-1) + a(m) for n >= 4, where m = n - 1 - 2^p and p is
 *          the unique integer such that 2^p < n - 1 <= 2^(p+1), starting with
 *          a(1) = a(2) = a(3) = 1.
 * https://oeis.org/A050026
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=a(2)=a(3)=1, a(n)=a(n-1)+a(n-1-2^p) (n≥4) where p is the
 *            unique integer with 2^p < n-1 ≤ 2^(p+1)
 * Range:     u8 n <= 46, u16 n <= 217, u32 n <= 1842, u64 n <= 8193,
 *            u128 n <= 8193, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
/* helper provided by the build */
# include "../utils/floor_log2_uint64.h"

/* cache of computed values, 1‑based */
static seq_t *cache      = NULL;
static idx_t  cache_cap  = 0;   /* allocated size                    */
static idx_t  max_computed = 0; /* maximum n for which a[n] is known */

static void ensure_cache_cap(idx_t need)
{
    /* ensure storage for indices 0..need inclusive */
    if (need < cache_cap) return;
    idx_t new_cap = need + 1;
    if (new_cap < 4) new_cap = 4;           /* at least enough for a(1..3) */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* if realloc fails, abort – build guarantees enough memory */
    if (!new_cache) abort();
    /* initialise new elements to zero to avoid garbage */
    for (idx_t i = cache_cap; i < new_cap; ++i) new_cache[i] = (seq_t)0;
    cache      = new_cache;
    cache_cap  = new_cap;
}

static void compute_up_to(idx_t n)
{
    if (n <= max_computed) return;
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t val;
        if (i <= 3) {
            val = (seq_t)1;
        } else {
            /* p = floor_log2(i-2) */
            idx_t p = floor_log2_uint64(i-2);
            idx_t m = (i-1) - ((idx_t)1 << p);
            val = cache[i-1] + cache[m];
        }
        cache[i] = val;
    }
    max_computed = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                 /* out of range, not used */
    if (!cache) {
        /* initialise cache for base values */
        ensure_cache_cap(3);
        cache[1] = cache[2] = cache[3] = (seq_t)1;
        max_computed = 3;
    }
    if (n >= cache_cap) {
        /* grow cache to accommodate n */
        ensure_cache_cap(n);
    }
    compute_up_to(n);
    return cache[n];
}
