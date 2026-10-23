/*
 * A016094  Expansion of 1/((1-9*x)*(1-10*x)*(1-11*x)*(1-12*x)).
 * https://oeis.org/A016094
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 23*a(n-1) - 132*a(n-2) + 10^(n+1) - 9^(n+1), a(0) = 1,
 *            a(1) = 42
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cache for already computed terms.  This makes repeated calls fast
   and allows non‑sequential access to a(n). */
static seq_t *Cache = NULL;
static size_t CacheCap = 0;
static idx_t CacheSize = 0;

/* Grow the cache to hold at least 'needed' elements. */
static void GrowCache(size_t needed)
{
    if (CacheCap >= needed)
        return;
    size_t new_cap = CacheCap ? CacheCap * 2 : 4;
    if (new_cap < needed)
        new_cap = needed;
    Cache = (seq_t *)realloc(Cache, new_cap * sizeof(seq_t));
    CacheCap = new_cap;
}

/* Initialise the cache with the base cases.  Called automatically. */
static void InitCache(void)
{
    GrowCache(2);
    Cache[0] = (seq_t)1;
    Cache[1] = (seq_t)42;
    CacheSize = 2;
}

seq_t a(idx_t n)
{
    /* Initialise on the first call. */
    if (Cache == NULL)
        InitCache();

    /* Return cached value if already computed. */
    if (n < CacheSize)
        return Cache[n];

    /* Compute powers up to n+1.  pow10 holds 10^k and pow9 holds 9^k,
       where k starts at CacheSize. */
    seq_t pow10 = (seq_t)1;
    seq_t pow9  = (seq_t)1;
    for (idx_t k = 0; k < CacheSize; ++k) {
        pow10 *= (seq_t)10;
        pow9  *= (seq_t)9;
    }

    /* Extend cache and compute new terms up to n. */
    GrowCache((size_t)n + 1);
    for (idx_t i = CacheSize; i <= n; ++i) {
        /* Update powers for 10^(i+1) and 9^(i+1). */
        pow10 *= (seq_t)10;
        pow9  *= (seq_t)9;

        /* Recurrence: a(i) = 23*a(i-1) - 132*a(i-2) + pow10 - pow9. */
        seq_t term = (seq_t)23 * Cache[i - 1];
        term -= (seq_t)132 * Cache[i - 2];
        term += pow10;
        term -= pow9;

        Cache[i] = term;
    }
    CacheSize = n + 1;

    return Cache[n];
}
