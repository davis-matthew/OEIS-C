/*
 * A026806  a(n) = number of numbers k such that only one partition of n has
 *          least part k.
 * https://oeis.org/A026806
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   for n>5, a(n) = a(n-2) + a(n-3) – a(n-5) Base values: a(1)=1,
 *            a(2)=2, a(3)=1, a(4)=2, a(5)=2 (These are the first terms of the
 *            sequence.)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoised values of a(n) */
static idx_t  cache_cap = 0;  /* allocated size of cache */
static idx_t  computed   = 0; /* largest n for which cache[n] is valid */

static void ensure_cache(idx_t n)
{
    /* Ensure cache has index up to n (i.e. size n+1) */
    idx_t needed = n + 1;
    if (cache_cap >= needed) return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) { abort(); }
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined, but guard */
    ensure_cache(n);

    /* Initialise base cases once */
    if (computed < 5) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)2;
        cache[5] = (seq_t)2;
        computed = 5;
    }

    if (n <= computed) return cache[n];

    /* Compute in order so that recurrence references are valid */
    for (idx_t i = computed + 1; i <= n; ++i) {
        cache[i] = cache[i - 2] + cache[i - 3] - cache[i - 5];
    }
    computed = n;
    return cache[n];
}
