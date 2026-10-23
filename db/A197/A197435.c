/*
 * A197435  Number of n X 1 0..2 arrays with no element equal to the average
 *          of its horizontal and vertical neighbors.
 * https://oeis.org/A197435
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 2*a(n-2) + 3*a(n-3) + 2*a(n-4)  for n > 5
   Initial values: a(1)=3, a(2)=6, a(3)=10, a(4)=28, a(5)=66 */

#include <stdlib.h>
#include <stddef.h>

/* static cache for computed values; 1‑based indexing, index 0 unused */
static seq_t *cache = NULL;
static size_t cache_size = 0;      /* number of elements allocated */
static size_t computed_up_to = 0;  /* largest n that has been computed */

/* ensure the cache has room for indices 0..needed-1 */
static void ensure_cache(size_t needed)
{
    if (cache_size < needed) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        /* if realloc fails, tmp == NULL and cache remains unchanged */
        cache = tmp;
        cache_size = needed;
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    /* handle very small n directly */
    if (n <= 5) {
        switch ((size_t)n) {
            case 1: return (seq_t)3;
            case 2: return (seq_t)6;
            case 3: return (seq_t)10;
            case 4: return (seq_t)28;
            case 5: return (seq_t)66;
        }
        /* unreachable */
        return (seq_t)0;
    }

    size_t N = (size_t)n;          /* N fits in size_t for the expected limits */
    size_t needed = N + 1;         /* we use 1‑based indexing; element 0 unused */

    ensure_cache(needed);

    /* initialise the base values if they aren't present yet */
    if (computed_up_to < 5) {
        size_t start = 1;
        size_t end   = computed_up_to < 5 ? 5 : computed_up_to;
        if (end > N) end = N;
        if (end >= 1) cache[1] = (seq_t)3;
        if (end >= 2) cache[2] = (seq_t)6;
        if (end >= 3) cache[3] = (seq_t)10;
        if (end >= 4) cache[4] = (seq_t)28;
        if (end >= 5) cache[5] = (seq_t)66;
        if (end > computed_up_to) computed_up_to = end;
    }

    /* compute missing terms up to N */
    for (size_t i = computed_up_to + 1; i <= N; ++i) {
        /* the recurrence only applies for i >= 6 */
        if (i < 6) continue;
        cache[i] = cache[i-1] + 2*cache[i-2] + 3*cache[i-3] + 2*cache[i-4];
    }

    computed_up_to = N;
    return cache[N];
}
