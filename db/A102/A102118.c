/*
 * A102118  Iccanobirt numbers (8 of 15): a(n) = R(a(n-1) + a(n-2) + a(n-3)),
 *          where R is the digit reversal function A004086.
 * https://oeis.org/A102118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0)=0, a(1)=0, a(2)=1; a(n)=rev(a(n-1)+a(n-2)+a(n-3)) for n ≥ 3
 * Range:     u8 n <= 12, u16 n <= 17, u32 n <= 27, u64 n <= 62, u128 n <= 108,
 *            big any n
 * Helpers:   rev, seq_malloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/rev.h"
#include "../utils/seq_malloc.h"
#include "../utils/seq_realloc.h"

/* Static cache for memoisation */
static seq_t *cache      = NULL;   /* array of computed terms          */
static idx_t  cache_cap  = 0;      /* current allocated capacity       */
static idx_t  cache_len  = 0;      /* number of terms actually stored */

 /* Initialise the cache with the base values */
static void init_cache(void)
{
    if (cache == NULL) {
        cache = seq_malloc(3);          /* allocate space for indices 0,1,2 */
        cache_cap = 3;
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 1;
        cache_len = 3;                  /* indices 0..2 are computed     */
    }
}

/* Ensure all terms up to n are computed */
static void ensure_computed(idx_t n)
{
    init_cache();                       /* guarantee cache exists */

    if (cache_len > n)
        return;                         /* already have up to n */

    /* Grow the cache to at least n+1 elements, doubling its size each time */
    idx_t new_cap = cache_cap;
    while (new_cap <= n) {
        new_cap <<= 1;                   /* double capacity              */
    }

    cache = seq_realloc(cache, new_cap);
    cache_cap = new_cap;

    /* Compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = cache[i-1] + cache[i-2] + cache[i-3];
        cache[i] = rev(sum);            /* digit reversal */
    }

    cache_len = n + 1;                  /* we now have up to n           */
}

/* Public function required by the build system */
seq_t a(idx_t n)
{
    ensure_computed(n);
    return cache[n];
}
