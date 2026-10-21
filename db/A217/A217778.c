/*
 * A217778  Expansion of (1-x)^2*(1-3*x)/((1-3*x+x^2)*(1-5*x+5*x^2)).
 * https://oeis.org/A217778
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A217778.
   Recurrence: a(0)=1, a(1)=3, a(2)=10, a(3)=34,
   a(n)=8*a(n-1)-21*a(n-2)+20*a(n-3)-5*a(n-4) for n>3. */

#include <stddef.h>
#include <stdlib.h>

/* We allow up to a few hundred thousand terms; 10000 is far below */
#define MAX_CACHE_SIZE 200000

static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* number of terms currently stored */
static size_t cache_size = 0;  /* allocated size */

/* Create the initial cache if it doesn't exist yet. */
static void init_cache(void)
{
    if (cache == NULL)
    {
        cache_size = MAX_CACHE_SIZE;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        if (cache == NULL)
            abort();                     /* out-of-memory – program can’t continue */

        /* Initial values of the sequence. */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)10;
        cache[3] = (seq_t)34;
        cache_cap = 4;
    }
}

/* Grow the cache to contain at least n+1 terms. */
static void grow_cache(size_t n)
{
    /* If the current allocation is enough, do nothing. */
    if (n + 1 <= cache_size)
        return;

    size_t new_size = cache_size * 2;
    if (new_size < n + 1)
        new_size = n + 1;           /* at least big enough for the required index */

    seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
    if (tmp == NULL)
        abort();                    /* out-of-memory */

    cache = tmp;
    cache_size = new_size;
}

/* Main entry point. */
seq_t a(idx_t n)
{
    init_cache();

    size_t idx = (size_t)n;
    if (idx < cache_cap)
        return cache[idx];

    grow_cache(idx);

    /* Compute missing terms by recurrence. */
    for (size_t i = cache_cap; i <= idx; ++i)
    {
        /* Use seq_t arithmetic; constants are cast to seq_t for type safety. */
        seq_t term = cache[i - 1] * (seq_t)8
                     - cache[i - 2] * (seq_t)21
                     + cache[i - 3] * (seq_t)20
                     - cache[i - 4] * (seq_t)5;
        cache[i] = term;
    }

    cache_cap = idx + 1;
    return cache[idx];
}
