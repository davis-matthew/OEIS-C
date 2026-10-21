/*
 * A066046  a(1) = 1; a(2) = 2; a(3) = 3; a(n+3) = a(n+2)*a(n+1) + a(n+1)*a(n)
 *          + a(n)*a(n+2).
 * https://oeis.org/A066046
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 8, u64 n <= 9, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=2, a(3)=3;
   a(n+3)=a(n+2)*a(n+1)+a(n+1)*a(n)+a(n)*a(n+2)  (n≥1) */
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;      /* 1‑based array, cache[0] unused */
static size_t cache_cap = 0;     /* allocated size (including index 0) */
static idx_t cache_nmax = 0;     /* highest index computed so far */

/* Grow the cache to hold all indices up to `need` (inclusive). */
static void ensure_capacity(size_t need)
{
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap == 0 ? 4 : cache_cap;
    while (new_cap < need)
        new_cap <<= 1;
#ifdef __cplusplus
    seq_t *new_cache = new seq_t[new_cap];      /* value‑initialized */
#else
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        return;                                /* out of memory – caller will error */
    /* initialize, only needed for C build; integers zero‑init already */
    for (size_t i = 0; i < new_cap; ++i)
        new_cache[i] = 0;
#endif
    if (cache) {
#ifdef __cplusplus
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        delete[] cache;
#else
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
        free(cache);
#endif
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialise base values if not already done. */
static void init_base(void)
{
    if (cache_nmax >= 3)
        return;
    ensure_capacity(4);                /* need indices 1..3 */
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)3;
    cache_nmax = 3;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;           /* undefined, but safe value */

    init_base();

    if (n <= cache_nmax)
        return cache[n];

    /* Ensure space for the new index.  +1 because indices are 1‑based */
    ensure_capacity((size_t)n + 1);

    /* Compute missing terms up to n. */
    for (idx_t i = cache_nmax + 1; i <= n; ++i) {
        /* a(i) = a(i-1)*a(i-2) + a(i-2)*a(i-3) + a(i-3)*a(i-1) */
        seq_t t1 = cache[i - 1] * cache[i - 2];
        seq_t t2 = cache[i - 2] * cache[i - 3];
        seq_t t3 = cache[i - 3] * cache[i - 1];
        cache[i] = t1 + t2 + t3;
    }
    cache_nmax = n;
    return cache[n];
}
