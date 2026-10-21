/*
 * A233139  Number of tilings of a 4 X n rectangle using T and Z tetrominoes.
 * https://oeis.org/A233139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(n-3) + 2*a(n-4) for n>3; a(0)=1,
 *            a(1)=a(2)=a(3)=0
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 29, u64 n <= 55, u128 n <= 107,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* array of computed values */
static idx_t  cache_size = 0; /* number of values stored (next free index) */
static size_t cache_cap  = 0; /* allocated capacity */

/* Allocate the cache (at least four elements) and store the base cases. */
static void init_cache(void)
{
    if (cache == NULL) {
        cache_cap  = 4;
        cache      = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0]   = 1;
        cache[1]   = 0;
        cache[2]   = 0;
        cache[3]   = 0;
        cache_size = 4;
    }
}

/* Grow the cache to at least new_cap elements. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap)
        return;
    cache_cap = (cache_cap > 0) ? cache_cap * 2 : 4;
    while (cache_cap < new_cap)
        cache_cap *= 2;
    seq_t *tmp = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    if (tmp)
        cache = tmp;
}

/* Public function computing the OEIS sequence A233139. */
seq_t a(idx_t n)
{
    /* Base cases for n < 4 are handled by init_cache. */
    if (n < 4) {
        init_cache();
        return cache[n];
    }

    init_cache();

    /* Extend cache up to n if necessary. */
    if (cache_size <= n) {
        grow_cache((size_t)n + 1);

        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t t1 = cache[i - 1];
            seq_t t2 = t1 + t1;          /* 2 * a(i-1) */
            seq_t t3 = cache[i - 3];     /* a(i-3) */
            seq_t t4 = cache[i - 4] + cache[i - 4]; /* 2 * a(i-4) */
            cache[i] = t2 + t3 + t4;
        }
        cache_size = n + 1;
    }

    return cache[n];
}
