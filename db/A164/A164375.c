/*
 * A164375  Number of reduced words of length n in Coxeter group on 9
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^7 = I.
 * https://oeis.org/A164375
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A164375
    a(0)=1, a(1)=9, a(2)=72, a(3)=576, a(4)=4608,
    a(5)=36864, a(6)=294912, a(7)=2359260.
    For n ≥ 8    a(n) = 7*(a(n‑1)+a(n‑2)+a(n‑3)+a(n‑4)+a(n‑5)+a(n‑6)) – 28*a(n‑7).  */

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;      /* cache of computed terms */
static size_t cache_cap = 0;     /* size of allocated cache */
static size_t cache_filled = 0;  /* number of initialized terms */

/* Ensure cache has room for at least 'needed' elements. */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed) new_cap *= 2;
    seq_t *p = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!p && new_cap > 0) exit(1);   /* allocation failure */
    cache = p;
    cache_cap = new_cap;
}

/* Compute a(n). */
seq_t a(idx_t n)
{
    /* Initialise first eight terms on first call. */
    if (cache_filled == 0) {
        grow_cache(8);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)9;
        cache[2] = (seq_t)72;
        cache[3] = (seq_t)576;
        cache[4] = (seq_t)4608;
        cache[5] = (seq_t)36864;
        cache[6] = (seq_t)294912;
        cache[7] = (seq_t)2359260;
        cache_filled = 8;
    }

    /* If requested value is already in cache. */
    if ((size_t)n < cache_filled) return cache[n];

    /* Grow cache to hold up to n. */
    if ((size_t)n + 1 > cache_cap) grow_cache((size_t)n + 1);

    /* Compute missing terms up to n. */
    for (idx_t i = cache_filled; i <= n; ++i) {
        /* Recurrence valid only for i ≥ 8. */
        seq_t sum6 = cache[i - 1]
                   + cache[i - 2]
                   + cache[i - 3]
                   + cache[i - 4]
                   + cache[i - 5]
                   + cache[i - 6];
        seq_t term = 7 * sum6;                /* 7 times the sum of the previous six */
        term -= 28 * cache[i - 7];            /* minus 28 times the term seven positions back */
        cache[i] = term;
    }
    cache_filled = (size_t)n + 1;
    return cache[n];
}
