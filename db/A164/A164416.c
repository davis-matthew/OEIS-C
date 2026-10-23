/*
 * A164416  Number of binary strings of length n with no substrings equal to
 *          0000 0001 or 1111.
 * https://oeis.org/A164416
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 19, u32 n <= 39, u64 n <= 79, u128 n <= 160,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A164416: Number of binary strings of length n with no substrings
 * equal to 0000, 0001 or 1111.
 *
 * Linear recurrence (from the denominator of the generating function)
 * a(n) = a(n-2) + 2*a(n-3) + 2*a(n-4) + a(n-5),  for n >= 9
 * with initial values
 *   a(4)=13, a(5)=23, a(6)=40, a(7)=69, a(8)=120.
 *
 * The function below memoises the sequence in a dynamic array.  All
 * calculations are performed in the type seq_t; no arithmetic is done
 * beyond what C supports for that type. */

#include <stdlib.h>
#include <string.h>

/* Static cache for already computed a(n).  cache[i] holds a(i). */
static seq_t *cache = NULL;       /* pointer to array of seq_t values   */
static idx_t cache_cap = 0;       /* current allocated size (>=0)       */
static idx_t cache_last = 3;      /* largest index that has a value set */

/* Initialise the cache with the known values a(4)…a(8). */
static void
init_cache(void)
{
    cache_cap = 9;                       /* indices 0 … 8 are allocated */
    cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    if (!cache) exit(1);

    /* a(n) = 0 for n < 4 (unused by final user, but kept for completeness). */
    cache[0] = cache[1] = cache[2] = cache[3] = (seq_t)0;
    cache[4] = (seq_t)13;
    cache[5] = (seq_t)23;
    cache[6] = (seq_t)40;
    cache[7] = (seq_t)69;
    cache[8] = (seq_t)120;

    cache_last = 8;                      /* last known index is 8 */
}

/* Ensure that the cache can store indices up to req (included). */
static void
ensure_cache(idx_t req)
{
    if (req + 1 <= cache_cap) return;   /* already large enough */

    idx_t new_cap = req + 1;             /* exact size needed */
    if (new_cap < 2 * cache_cap) new_cap = 2 * cache_cap; /* grow a bit */

    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
    if (!tmp) exit(1);

    /* Zero the newly allocated part if any. */
    if (new_cap > cache_cap)
        memset(tmp + cache_cap, 0, sizeof(seq_t) * (new_cap - cache_cap));

    cache     = tmp;
    cache_cap = new_cap;
}

seq_t
a(idx_t n)
{
    /* Undefined behaviour for n < 4 is allowed.  Return 0. */
    if (n < 4) return (seq_t)0;

    /* First call initialises the cache. */
    if (!cache) init_cache();

    /* Expand the cache if necessary. */
    if (n + 1 > cache_cap) ensure_cache(n);

    /* If the value has already been computed, return it. */
    if (n <= cache_last) return cache[n];

    /* Compute new terms using the recurrence.  The recurrence is valid
     * only for i >= 9; indices 4…8 are already stored. */
    for (idx_t i = cache_last + 1; i <= n; ++i)
    {
        /* We know i >= 9 because cache_last was 8 after init_cache. */
        seq_t val = cache[i - 5]                /* a(i‑5)   */
                 + 2 * cache[i - 4]           /* 2·a(i‑4) */
                 + 2 * cache[i - 3]           /* 2·a(i‑3) */
                 + cache[i - 2];              /* a(i‑2)   */
        cache[i] = val;
    }
    cache_last = n;
    return cache[n];
}
