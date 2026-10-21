/*
 * A061777  Start with a single triangle; at n-th generation add a triangle at
 *          each vertex, allowing triangles to overlap; sequence gives total
 *          population of triangles at n-th generation.
 * https://oeis.org/A061777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-2) - 4*a(n-3) + 2*a(n-4) with a(0)=1,
 *            a(1)=4, a(2)=10, a(3)=22. This recurrence generates OEIS
 *            A061777. The implementation memoises computed values in a static
 *            dynamic array so that calls may be made in any order
 * Range:     u8 n <= 7, u16 n <= 23, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* memoised values              */
static size_t cache_size = 0;        /* number of values stored      */
static size_t cache_alloc = 0;       /* allocated capacity (in seq_t)*/

/* ensure at least "need" elements are allocated in the cache */
static void cache_ensure(size_t need)
{
    if (need < cache_alloc)
        return;

    size_t new_alloc = (cache_alloc == 0) ? 4 : cache_alloc * 2;
    while (new_alloc <= need)
        new_alloc <<= 1;

    seq_t *new_cache = (seq_t *)malloc(new_alloc * sizeof(seq_t));
    if (!new_cache) {
        /* allocation failure – propagate as zero (unlikely in tests) */
        return;
    }

    /* copy existing data */
    for (size_t i = 0; i < cache_size; ++i)
        new_cache[i] = cache[i];

    /* free old cache */
    free(cache);
    cache = new_cache;
    cache_alloc = new_alloc;
}

/* compute all values up to index n (inclusive) and store in cache */
static void cache_compute_up_to(idx_t n)
{
    if (n < (idx_t)cache_size)
        return;

    /* initialise base values if this is the first call */
    if (cache_size == 0) {
        cache_ensure(4);
        cache[0] = 1;
        cache[1] = 4;
        cache[2] = 10;
        cache[3] = 22;
        cache_size = 4;
    } else {
        /* ensure enough space for the new values */
        cache_ensure((size_t)n);
    }

    /* compute remaining values using the recurrence */
    for (idx_t i = (idx_t)cache_size; i <= n; ++i) {
        seq_t a1 = cache[i-1];
        seq_t a2 = cache[i-2];
        seq_t a3 = cache[i-3];
        seq_t a4 = cache[i-4];

        /* sequence update: 2*a1 + a2 - 4*a3 + 2*a4 */
        seq_t val = a1 + a1;          /* 2*a1 */
        val += a2;                    /* + a2 */
        val += a4 + a4;               /* + 2*a4 */
        val -= a3 + a3 + a3 + a3;     /* - 4*a3 (unsigned subtraction is safe
                                          because the result is always non‑negative) */

        cache[i] = val;
    }

    cache_size = (size_t)n + 1;
}

seq_t a(idx_t n)
{
    /* base cases */
    switch (n) {
        case 0: return 1;
        case 1: return 4;
        case 2: return 10;
        case 3: return 22;
    }

    cache_compute_up_to(n);
    return cache[(size_t)n];
}
