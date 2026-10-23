/*
 * A189738  a(1)=3, a(2)=4, a(n) = 3*a(n-1) + 4*a(n-2).
 * https://oeis.org/A189738
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=3, a(2)=4, a(n)=3*a(n-1)+4*a(n-2) */

#include <stdlib.h>
#include <stddef.h>

/* static cache for memoization */
static seq_t *cache      = NULL;
static idx_t cache_len   = 0;   /* highest index currently computed */
static size_t cache_cap = 0;    /* current capacity of the cache */

/* ensure that the cache contains values up to index n */
static void ensure_cache_size(idx_t n)
{
    if (cache == NULL)  /* first call: allocate and initialise base values */
    {
        cache_cap = 64;  /* initial capacity */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)4;
        cache_len = 2;
    }

    if (n <= cache_len) return;  /* already computed */

    /* grow capacity if necessary */
    while ((idx_t)cache_cap < n + 1)
        cache_cap <<= 1;  /* double until big enough */

    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;

    /* compute missing terms */
    for (idx_t i = cache_len + 1; i <= n; ++i)
    {
        seq_t prev  = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        cache[i] = (seq_t)3 * prev + (seq_t)4 * prev2;
    }
    cache_len = n;
}

/* sequence A189738 */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* offset protection, unused */
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)4;
    ensure_cache_size(n);
    return cache[n];
}
