/*
 * A185590  Iterate the map in A006369 starting at 44.
 * https://oeis.org/A185590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Map: a(1)=44; a(n)=next_term(a(n‑1)) where next_term is the map from A006369. */
#include <stdlib.h>
#ifdef __cplusplus
#include <utility>
#endif

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t max_cached = 0;

static void ensure_cache_cap(size_t needed)
{
#ifdef __cplusplus
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *newc = new seq_t[new_cap];
    if (cache) {
        // Move existing elements to new storage
        std::move(cache, cache + max_cached, newc);
        delete [] cache;
    }
    cache = newc;
    cache_cap = new_cap;
#else
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *newc = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!newc && new_cap) {
        /* allocation failure - abort */
        abort();
    }
    cache = newc;
    cache_cap = new_cap;
#endif
}

static seq_t next_term(seq_t x)
{
    seq_t q = x / 3;
    seq_t r = x % 3;
    if (r == 0)
        return x - q;
    else if (r == 1)
        return x + q;
    else
        return x + q + 1;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* behavior below offset is irrelevant */

    /* Ensure storage can hold n terms */
    ensure_cache_cap((size_t)n);

    /* Initialise the first term once */
    if (max_cached == 0) {
        cache[0] = (seq_t)44;
        max_cached = 1;
    }

    /* Generate any missing terms up to n */
    seq_t val = cache[max_cached - 1];
    for (idx_t i = max_cached; i < n; ++i) {
        val = next_term(val);
        cache[i] = val;
    }
    max_cached = n;

    return cache[n - 1];
}
