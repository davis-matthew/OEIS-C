/*
 * A185357  Expansion of 1/(1 - x - x^2 + x^18 - x^20).
 * https://oeis.org/A185357
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; For n >= 1, if n < 18: a(n) = a(n-1) + a(n-2); else if n
 *            < 20: a(n) = a(n-1) + a(n-2) - a(n-18); else: a(n) = a(n-1) +
 *            a(n-2) - a(n-18) + a(n-20)
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 185,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/alloc_seq.h"   /* alloc_seq */
#include "../utils/free_seq.h"    /* free_seq */

/* Static cache for the sequence */
static seq_t *cache = NULL;      /* array of computed terms   */
static size_t cache_cap = 0;     /* number of elements allocated */
static idx_t  max_computed = 0;  /* highest index whose value is known */

/* Ensure that the cache can hold values up to index n (inclusive). */
static void
ensure_cache(idx_t n)
{
    /* need capacity = n+1 */
    size_t needed = (size_t)n + 1;
    if (cache_cap >= needed)
        return;                /* already enough */

    size_t new_cap = cache_cap ? cache_cap * 2 : 20;
    while (new_cap < needed)
        new_cap *= 2;

    seq_t *new_cache = alloc_seq(new_cap);
    if (!new_cache)
        abort();               /* allocation failure */

    /* copy existing entries */
    for (size_t i = 0; i < cache_cap; ++i)
        new_cache[i] = cache[i];

    /* free old storage if any */
    if (cache)
        free_seq(cache);

    cache      = new_cache;
    cache_cap  = new_cap;
}

/* Compute all missing terms up to index n (inclusive). */
static void
build_to(idx_t n)
{
    if (cache == NULL) {
        /* first allocation and initialisation of the first 20 terms */
        ensure_cache(19);
        cache[0] = (seq_t)1; /* a(0) = 1 */

        /* Compute terms 1..19 using the recurrence. */
        for (idx_t i = 1; i <= 19; ++i) {
            seq_t s = cache[i - 1];
            if (i >= 2)
                s += cache[i - 2];
            if (i >= 18)
                s -= cache[i - 18];
            if (i >= 20)   /* never true for i <= 19 */
                s += cache[i - 20];
            cache[i] = s;
        }
        max_computed = 19;
    }

    if (n <= max_computed)
        return;   /* nothing to do */

    ensure_cache(n);

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t s = cache[i - 1];
        if (i >= 2)
            s += cache[i - 2];
        if (i >= 18)
            s -= cache[i - 18];
        if (i >= 20)
            s += cache[i - 20];
        cache[i] = s;
    }
    max_computed = n;
}

/* Public function required by the problem. */
seq_t a(idx_t n)
{
    build_to(n);
    return cache[n];
}
