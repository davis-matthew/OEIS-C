/*
 * A060521  Number of 3 X n grids of black and white cells, no 3 of same color
 *          vertically or horizontally contiguous.
 * https://oeis.org/A060521
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 17, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=6, a(2)=36, a(3)=102, a(4)=378,
   a(n)=2*a(n-1)+5*a(n-2)-a(n-4) for n>=5 */
#include <stdlib.h>

static seq_t *cache = NULL;   /* 1‑based cache */
static size_t cache_cap = 0;  /* allocated capacity (>=1) */
static idx_t cached_up_to = 0;   /* largest n for which cache[n] is valid */

/* Ensure cache has capacity for indices up to n (inclusive). */
static void ensure_cache_capacity(idx_t n)
{
    /* Need space for indices 0..n inclusive; index 0 unused, so n+1 elements. */
    size_t needed = (size_t)(n + 1);
    if (cache_cap >= needed) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < needed) new_cap = needed;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    /* If realloc fails, we let the program abort (as memory exhaustion). */
    cache = new_cache;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined for n=0, but return zero */

    ensure_cache_capacity(n);

    /* Fill missing initial values and compute new terms. */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        if (i == 1)
            cache[i] = (seq_t)6;
        else if (i == 2)
            cache[i] = (seq_t)36;
        else if (i == 3)
            cache[i] = (seq_t)102;
        else if (i == 4)
            cache[i] = (seq_t)378;
        else
            cache[i] = (seq_t)( (seq_t)2 * cache[i-1] + (seq_t)5 * cache[i-2] - cache[i-4] );
    }
    cached_up_to = n;   /* up to now, cache[n] is ready */
    return cache[n];
}
