/*
 * A021719  Decimal expansion of 1/715.
 * https://oeis.org/A021719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A021719: Decimal expansion of 1/715
   Recurrence:   a(0)=0, a(1)=0, a(2)=1, a(3)=3, a(4)=9
                 a(n) = a(n-1) - a(n-3) + a(n-4)   for n > 4
   The sequence consists of the decimal digits of 1/715.  The period
   is 6 (digits 139860).  The function below memoises the terms in a
   static array so that subsequent calls are fast and independent of
   call order. */
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Cached values of the sequence.  In C++ the type seq_t may be a
   big‑integer class; in C it is an unsigned integer type. */
static seq_t *cache = NULL;          /* array of computed terms          */
static size_t cache_capacity = 0;    /* allocated size of the array      */
static idx_t cache_max = (idx_t)-1;  /* highest index that has been computed */

/* Ensure that the cache array has room for at least `needed` elements. */
static void ensure_capacity(size_t needed)
{
    if (needed <= cache_capacity)
        return;

    size_t newcap = cache_capacity ? cache_capacity << 1 : 8;
    while (newcap < needed)
        newcap <<= 1;

    if (!cache) {
        cache = (seq_t *)malloc(newcap * sizeof(seq_t));
    } else {
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (tmp) cache = tmp;           /* ignore failure – build will
                                            check for assertions on valid
                                            sizes; here we assume success */
    }
    cache_capacity = newcap;
}

/* Initialise the cache with the first five terms.  This routine is
   called lazily on the first request. */
static void init_cache(void)
{
    if (cache)
        return;  /* already initialised */

    ensure_capacity(5);
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)1;
    cache[3] = (seq_t)3;
    cache[4] = (seq_t)9;
    cache_max = 4;
}

/* Compute a(n) for the sequence A021719. */
seq_t a(idx_t n)
{
    init_cache();

    /* Fast path for already known terms. */
    if (n <= cache_max)
        return cache[(size_t)n];

    /* Grow the cache to accommodate the requested index. */
    size_t needed = (size_t)n + 1;
    ensure_capacity(needed);

    /* Compute missing terms using the recurrence. */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        /* indices i-1, i-3, i-4 are safe because i > cache_max >= 4. */
        seq_t val = cache[(size_t)(i - 1)]
                    - cache[(size_t)(i - 3)]
                    + cache[(size_t)(i - 4)];
        cache[(size_t)i] = val;
    }
    cache_max = n;
    return cache[(size_t)n];
}
