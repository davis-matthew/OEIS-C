/*
 * A028129  Expansion of 1/((1-4x)(1-5x)(1-11x)(1-12x)).
 * https://oeis.org/A028129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A028129: Expansion of 1/((1-4x)(1-5x)(1-11x)(1-12x))
   Formula: a(n) = 32*a(n-1) - 359*a(n-2) + 1648*a(n-3) - 2640*a(n-4)
   Initial values: a(0)=1, a(1)=32, a(2)=665, a(3)=11440 */

#include <stdlib.h>
#include <stddef.h>

/* static cache for memoisation */
static seq_t *cache = NULL;      /* array of already computed terms          */
static size_t cache_size = 0;    /* number of terms stored in the cache      */
static size_t cache_cap  = 0;    /* capacity of the allocated array          */

/* ensure the cache can hold indices up to n (inclusive) */
static void
ensure_cache(idx_t n)
{
    if (cache_cap > (size_t)n) return;          /* already enough capacity */

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap <= (size_t)n) new_cap *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                          /* out of memory */
    cache = tmp;
    cache_cap = new_cap;
}

/* compute a(n) for any n >= 0, using memoisation */
seq_t
a(idx_t n)
{
    /* Make sure the cache can address n and the 4 preceding elements */
    ensure_cache(n);

    /* initialise the first four terms if not yet done */
    if (cache_size < 4) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)32;
        cache[2] = (seq_t)665;
        cache[3] = (seq_t)11440;
        cache_size = 4;
    }

    /* compute any missing terms up to n */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t t1 = cache[i-1];
        t1 *= (seq_t)32;               /* 32*a(i-1) */

        seq_t t2 = cache[i-2];
        t2 *= (seq_t)359;              /* 359*a(i-2) */

        seq_t t3 = cache[i-3];
        t3 *= (seq_t)1648;             /* 1648*a(i-3) */

        seq_t t4 = cache[i-4];
        t4 *= (seq_t)2640;             /* 2640*a(i-4) */

        seq_t val = t1 - t2 + t3 - t4; /* assemble the recurrence */
        cache[i] = val;
    }

    cache_size = (size_t) (n + 1);    /* all terms up to n are now cached */
    return cache[n];
}
