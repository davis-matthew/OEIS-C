/*
 * A086835  a(n) = a( (n - a(n-1))*(n mod 2) + a(n-1)*(1 - (n mod 2)) ) + a(
 *          (n - a(n-2))*(n mod 2) + (n - a(n-1))*(1 - (n mod 2)) ), with a(1)
 *          = a(2) = 1.
 * https://oeis.org/A086835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=1, for n>=3 a(n) = a( (n - a(n-1))*(n mod 2) +
 *            a(n-1)*(1 - (n mod 2)) ) + a( (n - a(n-2))*(n mod 2) + (n -
 *            a(n-1))*(1 - (n mod 2)) ) (the indices are 1‑based, but treat
 *            indices 0 as unused)
 * Range:     u8 n <= 363, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;  /* 1‑based array of computed values */
static idx_t  cache_cap  = 0;/* allocated number of elements (>=1) */
static idx_t  cache_max  = 0;/* largest n for which a(n) is known */

/* Ensure that cache can hold indices up to n (1‑based). */
static void ensure_cache(idx_t n)
{
    if (cache_cap > n)
        return;
    idx_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap <= n)
        new_cap <<= 1;
    /* realloc returns void* in C and requires a cast in C++ */
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)            /* allocation failed – keep old cache */
        return;
    cache = new_cache;
    /* zero new entries (ignoring index 0) */
    for (idx_t i = cache_cap; i < new_cap; ++i)
        cache[i] = 0;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1)          /* undefined for n < 1; return 0 as safe value */
        return 0;

    ensure_cache(n);

    /* initialise base terms if necessary */
    if (cache_max < 2) {
        cache[1] = 1;
        cache[2] = 1;
        cache_max = 2;
    }

    /* If requested n already known, return it immediately */
    if (n <= cache_max)
        return cache[n];

    /* Compute values from cache_max+1 up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        idx_t idx1, idx2;
        if (i & 1) {                    /* odd i  -> n mod 2 = 1 */
            idx1 = i - (idx_t)cache[i - 1];
            idx2 = i - (idx_t)cache[i - 2];
        } else {                        /* even i -> n mod 2 = 0 */
            idx1 = (idx_t)cache[i - 1];
            idx2 = i - (idx_t)cache[i - 1];
        }
        /* indices are guaranteed to be >= 1 by the recurrence definition */
        cache[i] = cache[idx1] + cache[idx2];
    }
    cache_max = n;
    return cache[n];
}
