/*
 * A024446  Expansion of 1/((1-x)(1-7x)(1-11x)(1-12x)).
 * https://oeis.org/A024446
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=31, a(2)=638, a(3)=10982; for n>=4:
 *            a(n)=31*a(n-1)-323*a(n-2)+1217*a(n-3)-924*a(n-4)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mul_small.h"

static seq_t *cache = NULL;            /* cached terms */
static idx_t  cache_size = 0;          /* number of terms currently cached */
static idx_t  cache_cap   = 0;          /* allocated capacity */

/* Ensure the cache holds at least [n+1] terms */
static void ensure_cache(idx_t n)
{
    if (cache_cap > 0 && cache_cap > n) return;

    idx_t new_cap = (n + 1) * 2;
    if (new_cap < 16) new_cap = 16;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache) { /* allocation failed – terminate */
        abort();
    }
    cache = new_cache;
    cache_cap = new_cap;

    /* Initialise first 4 terms if not yet done */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)31;
        cache[2] = (seq_t)638;
        cache[3] = (seq_t)10982;
        cache_size = 4;
    }
}

seq_t a(idx_t n)
{
    if (n < 4) {
        /* Handle small indices without caching */
        if (n == 0) return (seq_t)1;
        if (n == 1) return (seq_t)31;
        if (n == 2) return (seq_t)638;
        return (seq_t)10982;
    }

    ensure_cache(n);

    if (n < cache_size) return cache[n];

    /* Compute terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t t1 = mul_small(cache[i - 1], 31);
        seq_t t2 = mul_small(cache[i - 3], 1217);
        seq_t sum1 = t1 + t2;

        seq_t u1 = mul_small(cache[i - 2], 323);
        seq_t u2 = mul_small(cache[i - 4], 924);
        seq_t sum2 = u1 + u2;

        /* sum1 >= sum2 for all n >= 4, ensure no underflow */
        cache[i] = sum1 - sum2;
    }
    cache_size = n + 1;
    return cache[n];
}
