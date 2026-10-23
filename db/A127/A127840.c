/*
 * A127840  a(1)=1, a(2)=...=a(6)=0, a(n) = a(n-6)+a(n-5) for n>6.
 * https://oeis.org/A127840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 65, u16 n <= 108, u32 n <= 195, u64 n <= 371,
 *            u128 n <= 722, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Sequence A127840
   Recurrence: a(1) = 1, a(2)=…=a(6)=0,
   a(n) = a(n-6) + a(n-5)   for n > 6
   (Offset 1) */

static seq_t *cache = NULL;     /* array: indices 0…cache_alloc-1 */
static size_t cache_alloc = 0;  /* allocated size (number of slots) */
static size_t cache_filled = 0; /* greatest index whose value is known */

/* Initialise cache with the first 6 terms (a(1)=1, a(2)…a(6)=0). */
static void init_cache(void)
{
    if (cache) return;                     /* already initialised */
    cache_alloc = 7;                       /* we need indices 0…6 */
    cache = (seq_t *)calloc(cache_alloc, sizeof(seq_t));
    if (!cache) return;                    /* allocation failed – fall back */
    cache[1] = (seq_t)1;                   /* a(1) = 1; indices 2‑6 stay 0 */
    cache_filled = 6;                      /* we have values up to 6   */
}

/* Grow the cache so that index n can be stored.  New slots are zeroed. */
static void extend_cache_to(idx_t n)
{
    if (!cache) init_cache();
    if ((idx_t)cache_alloc - 1 >= n) return;   /* already big enough */

    size_t new_alloc = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
    if (!tmp) return;                      /* realloc failed – keep old */
    cache = tmp;
    /* initialise new entries to zero */
    for (size_t i = cache_alloc; i < new_alloc; ++i) cache[i] = (seq_t)0;
    cache_alloc = new_alloc;
}

/* Compute all terms up to n (inclusive) that are not yet known. */
static void compute_upto(idx_t n)
{
    if (!cache) init_cache();
    if (cache_filled >= (size_t)n) return;  /* already computed */

    /* Start filling from one plus the last known index. */
    for (idx_t i = (idx_t)cache_filled + 1; i <= n; ++i)
    {
        if (i <= 6)          /* base cases: a(2)..a(6) are 0 already */
            cache[(size_t)i] = (seq_t)0;
        else
            cache[(size_t)i] =
                cache[(size_t)(i-6)] + cache[(size_t)(i-5)];
    }
    cache_filled = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;  /* undefined for n < 1 */
    extend_cache_to(n);
    compute_upto(n);
    return cache[(size_t)n];
}
