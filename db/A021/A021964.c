/*
 * A021964  Expansion of g.f. 1/((1-x)*(1-4*x)*(1-9*x)*(1-11*x)).
 * https://oeis.org/A021964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=25, a(2)=422, a(3)=6050;
   for n>3: a(n)=25*a(n-1)-203*a(n-2)+575*a(n-3)-396*a(n-4) */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Ensure the cache is allocated and the first four terms are set. */
static void a_init(void)
{
    if (cache_size == 0)
    {
        cache_size = 4;
        cache = (seq_t *)calloc(cache_size, sizeof(seq_t));
        if (!cache) return;          /* allocation failed – return 0 later */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)25;
        cache[2] = (seq_t)422;
        cache[3] = (seq_t)6050;
    }
}

seq_t a(idx_t n)
{
    a_init();

    /* If n already in cache, return it. */
    if (n < cache_size)
        return cache[n];

    /* Grow the cache to size n+1. */
    seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!new_cache)        /* allocation failed – return 0 */
        return (seq_t)0;
    cache = new_cache;

    /* Initialize new cache entries. */
    for (idx_t i = cache_size; i <= n; ++i)
        cache[i] = (seq_t)0;

    /* Compute values from cache_size up to n. */
    for (idx_t i = cache_size; i <= n; ++i)
    {
        /* All arithmetic is performed in seq_t; intermediate results
           are mathematically non‑negative, so unsigned overflow will
           not occur for values that fit in seq_t. */
        seq_t val = (seq_t)25 * cache[i - 1];
        val -= (seq_t)203 * cache[i - 2];
        val += (seq_t)575 * cache[i - 3];
        val -= (seq_t)396 * cache[i - 4];
        cache[i] = val;
    }

    cache_size = n + 1;
    return cache[n];
}
