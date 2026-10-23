/*
 * A041177  Denominators of continued fraction convergents to sqrt(98).
 * https://oeis.org/A041177
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041177:
 *  a(0)=1, a(1)=1, a(2)=9, a(3)=10,
 *  a(4)=189, a(5)=199, a(6)=1781, a(7)=1980
 *  a(n) = 198*a(n-4) - a(n-8)   for n >= 8
 *  Implements memoised recurrence; uses dynamic array that grows.
 *  Works for unsigned integer types and for arbitrary‑precision integer
 *  classes (compiled as C++17).
 */

#include <stdlib.h>
#ifdef __cplusplus
#include <new>
#endif

/* Static cache for computed terms. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;      /* allocated size of cache */
static idx_t   max_computed = 7;  /* largest index that has been computed */

/* Ensure cache has space for indices up to `need`.  If larger than
 * current capacity, reallocates (doubling strategy) and copies existing
 * values.  Works for both built‑in integer types and for C++ arbitrary
 * precision types. */
static void ensure_cache_cap(idx_t need)
{
    if ((size_t)need < cache_cap)               /* already large enough */
        return;

    /* Determine new capacity: start at 8 or double until > need. */
    size_t new_cap = cache_cap ? cache_cap : 8;
    while (new_cap <= (size_t)need)
        new_cap *= 2;

    /* Allocate new array. */
    seq_t *new_cache = NULL;
#ifdef __cplusplus
    new_cache = new seq_t[new_cap];
#else
    new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
#endif

    /* Copy old values if any, then free old array. */
    if (cache != NULL) {
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
#ifdef __cplusplus
        delete[] cache;
#else
        free(cache);
#endif
    }

    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialise cache with the first 8 terms.  Called lazily on first use. */
static void init_cache(void)
{
    ensure_cache_cap(7);          /* need space for indices 0..7 */

    cache[0] = (seq_t)1;
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)9;
    cache[3] = (seq_t)10;
    cache[4] = (seq_t)189;
    cache[5] = (seq_t)199;
    cache[6] = (seq_t)1781;
    cache[7] = (seq_t)1980;
}

/* Compute a(n) for any n >= 0.  Uses memoisation; recomputes only missing
 * terms.  Works correctly for any calling order. */
seq_t a(idx_t n)
{
    /* Lazy initialisation. */
    if (cache == NULL)
        init_cache();

    /* If the term is already computed, just return it. */
    if (n <= max_computed)
        return cache[n];

    /* Ensure we have enough allocated space. */
    ensure_cache_cap(n);

    /* Compute missing terms up to n. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        cache[i] = (seq_t)198 * cache[i - 4] - cache[i - 8];
    }

    max_computed = n;
    return cache[n];
}
