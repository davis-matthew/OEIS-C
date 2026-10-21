/*
 * A237013  Number of (n+1)X(6+1) 0..3 arrays with the sum of all four
 *          elements of every 2X2 subblock equal.
 * https://oeis.org/A237013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the 15‑term linear recurrence a(n) = 8·a(n‑1) +
 *            16·a(n‑2) – 268·a(n‑3) + 189·a(n‑4) + 3388·a(n‑5) – 5824·a(n‑6)
 *            – 20048·a(n‑7) + 49756·a(n‑8) + 52752·a(n‑9) – 196896·a(n‑10) –
 *            24192·a(n‑11) + 369216·a(n‑12) – 131328·a(n‑13) – 262656·a(n‑14)
 *            + 165888·a(n‑15) with initial values a(1)...
 * Range:     u8 none, u16 n <= 1, u32 n <= 13, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;          /* cache[0] unused, cache[i] = a(i) */
static size_t cache_cap = 0;          /* allocated size of cache (>= cached_n+1) */
static idx_t cached_n = 0;            /* largest n for which a(n) is in cache */

/* Initialise cache with the first fifteen terms. */
static void init_cache(void)
{
    if (cache_cap != 0)
        return;                       /* already initialised */

    cache_cap = 16;                   /* indices 0..15 (0 unused) */
    cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
    if (!cache)
        abort();

    cache[0] = 0;
    cache[1] = 61472;
    cache[2] = 92328;
    cache[3] = 149608;
    cache[4] = 282048;
    cache[5] = 566936;
    cache[6] = 1274544;
    cache[7] = 2995048;
    cache[8] = 7603440;
    cache[9] = 19947032;
    cache[10] = 55347408;
    cache[11] = 157602088;
    cache[12] = 468497328;
    cache[13] = 1423485656;
    cache[14] = 4480816464;
    cache[15] = 14392051048;

    cached_n = 15;
}

/* Grow the cache to at least `needed` elements (indices 0..needed-1). */
static void grow_cache(size_t needed)
{
    if (needed < cache_cap)
        return;

    size_t new_cap = cache_cap;
    while (new_cap <= needed)
        new_cap *= 2;                 /* exponential growth */

    seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();

    cache = new_cache;
    /* new slots are automatically uninitialized; set them to zero for safety */
    for (size_t i = cache_cap; i < new_cap; ++i)
        cache[i] = 0;

    cache_cap = new_cap;
}

/* Ensure that a(k) is computed for all k ≤ n. */
static void ensure_cache(idx_t n)
{
    if (n <= cached_n)
        return;

    /* Grow capacity if needed.  We need indices up to n. */
    grow_cache((size_t)n + 1);

    /* Compute new terms using the recurrence. */
    for (idx_t i = cached_n + 1; i <= n; ++i)
    {
        seq_t term = 0;
        term += (seq_t)8      * cache[(size_t)(i-1)];
        term += (seq_t)16     * cache[(size_t)(i-2)];
        term -= (seq_t)268    * cache[(size_t)(i-3)];
        term += (seq_t)189    * cache[(size_t)(i-4)];
        term += (seq_t)3388   * cache[(size_t)(i-5)];
        term -= (seq_t)5824   * cache[(size_t)(i-6)];
        term -= (seq_t)20048  * cache[(size_t)(i-7)];
        term += (seq_t)49756  * cache[(size_t)(i-8)];
        term += (seq_t)52752  * cache[(size_t)(i-9)];
        term -= (seq_t)196896 * cache[(size_t)(i-10)];
        term -= (seq_t)24192  * cache[(size_t)(i-11)];
        term += (seq_t)369216 * cache[(size_t)(i-12)];
        term -= (seq_t)131328 * cache[(size_t)(i-13)];
        term -= (seq_t)262656 * cache[(size_t)(i-14)];
        term += (seq_t)165888 * cache[(size_t)(i-15)];
        cache[(size_t)i] = term;
    }

    cached_n = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;          /* value below the offset is undefined */

    init_cache();
    ensure_cache(n);
    return cache[(size_t)n];
}
