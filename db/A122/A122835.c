/*
 * A122835  Number of topologies on n labeled elements in which no element
 *          belongs to any pair of noncomparable members of the topology.
 * https://oeis.org/A122835
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = 2^(n-1) – 1 + Σ{k=1..n} C(n,k) * a(n-k), with a(0)=1
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses memoisation.  It works for any
 * seq_t/idx_t pair supplied by the build (fixed‑width bit‑sized
 * or arbitrary precision).  The routine pow2(idx_t) is available
 * from the utils module and returns 2^exp as a seq_t. */
#include <stdlib.h>
#include <string.h>
#include "../utils/pow2.h"

#ifndef __cplusplus
/* Plain C version – cache is a dynamically allocated array */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;

static void ensure_cache(idx_t n)
{
    if (cache_cap > n) return;

    /* Grow to a power of two that can hold n+1 entries */
    idx_t new_cap = ((n + 1) < 16) ? 16 : (n + 1);
    while (new_cap <= n) new_cap <<= 1;

    seq_t *new_mem = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_mem) return;                     /* allocation failed – do nothing */

    /* Copy old data if any */
    if (cache) {
        memcpy(new_mem, cache, cache_cap * sizeof(seq_t));
        free(cache);
    }

    /* Zero the newly allocated area */
    memset(new_mem + cache_cap, 0, (new_cap - cache_cap) * sizeof(seq_t));
    cache = new_mem;
    cache_cap = new_cap;

    /* Initialise a(0) to 1 if it has not been set yet */
    if (cache_cap > 0 && cache[0] == (seq_t)0) cache[0] = (seq_t)1;
}
#else
/* C++ version – use a vector as cache */
#include <vector>
static std::vector<seq_t> cache;

static void ensure_cache(idx_t n)
{
    if (cache.size() > n) return;
    cache.resize(n + 1, (seq_t)0);
    if (cache.size() > 0 && cache[0] == (seq_t)0) cache[0] = (seq_t)1;
}
#endif

seq_t a(idx_t n)
{
    ensure_cache(n);

#ifdef __cplusplus
    /* Compute all values up to n that are still missing. */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != (seq_t)0) continue;          /* already known */

        seq_t sum = 0;
        seq_t bin = 1;                               /* C(i,0) */

        for (idx_t k = 1; k <= i; ++k) {
            if (k == 1) bin = (seq_t)i;              /* C(i,1) = i */
            else bin = (bin * (seq_t)(i - k + 1)) / (seq_t)k;

            sum += bin * cache[i - k];
        }

        cache[i] = pow2(i - 1) - (seq_t)1 + sum;
    }
    return cache[n];
#else
    /* Same logic for C. */
    for (idx_t i = 1; i <= n; ++i) {
        if (cache[i] != (seq_t)0) continue;          /* already known */

        seq_t sum = 0;
        seq_t bin = 1;                               /* C(i,0) */

        for (idx_t k = 1; k <= i; ++k) {
            if (k == 1) bin = (seq_t)i;              /* C(i,1) = i */
            else bin = (bin * (seq_t)(i - k + 1)) / (seq_t)k;

            sum += bin * cache[i - k];
        }

        cache[i] = pow2(i - 1) - (seq_t)1 + sum;
    }
    return cache[n];
#endif
}
