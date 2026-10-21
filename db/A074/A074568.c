/*
 * A074568  a(n) = 4^n + 7^n + 8^n.
 * https://oeis.org/A074568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4^n + 7^n + 8^n Recurrence: a(0)=3, a(1)=19, a(2)=129,
 *            a(n)=19*a(n-1)-116*a(n-2)+224*a(n-3) for n≥3
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>     /* malloc, realloc, abort, size_t */

static seq_t *cache   = NULL;   /* pointer to cached terms */
static size_t cache_len = 0;    /* number of cached terms          */
static size_t cache_cap = 0;    /* allocated capacity in terms      */

/* Ensure the cache has room for at least new_cap elements. */
static void ensure_cache_cap(size_t new_cap)
{
    if (cache_cap >= new_cap)
        return;

    /* realloc returns void*; cast explicitly for C++ compatibility */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* allocation failure – terminate */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Ensure the cache contains the first three base terms. */
static void ensure_cache_base(void)
{
    if (cache_len >= 3)
        return;

    /* Allocate space for the first three terms */
    size_t need = 3;
    if (cache_cap < need)
        ensure_cache_cap(need);

    cache[0] = (seq_t)3;
    cache[1] = (seq_t)19;
    cache[2] = (seq_t)129;
    cache_len = 3;
}

/* Compute a(n) for any non‑negative n. */
seq_t a(idx_t n)
{
    /* Convert the 64‑bit index to size_t for indexing.  The value of n
       is always non–negative, so the cast is safe on all supported
       platforms. */
    size_t target = (size_t)n;   /* index is zero‑based */

    /* Ensure base terms are cached. */
    ensure_cache_base();

    /* Grow cache if necessary. */
    if (target >= cache_len) {
        size_t new_cap = target + 1; /* enough for index n */
        if (cache_cap < new_cap)
            ensure_cache_cap(new_cap);

        for (size_t i = cache_len; i <= target; ++i) {
            if (i < 3) {
                /* base terms already initialized */
                continue;
            }
            /* Linear recurrence for i ≥ 3 */
            cache[i] = (seq_t)19 * cache[i - 1]
                     - (seq_t)116 * cache[i - 2]
                     + (seq_t)224 * cache[i - 3];
        }
        cache_len = target + 1;
    }

    return cache[target];
}
