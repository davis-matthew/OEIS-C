/*
 * A078052  Expansion of (1-x)/(1+x+2*x^2+2*x^3).
 * https://oeis.org/A078052
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) - 2*a(n-2) - 2*a(n-3), a(0)=1, a(1)=-2, a(2)=0
 * Range:     i8 n <= 14, i16 n <= 30, i32 n <= 62, i64 n <= 126,
 *            i128 n <= 254, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;          /* array of computed terms */
static idx_t   cache_size = 0;       /* number of valid entries in cache */
static idx_t   cache_cap   = 0;       /* allocated capacity of cache */

/* extend the cache to at least 'needed' entries */
static void ensure_cache_cap(idx_t needed)
{
    if (needed <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < needed)
        new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/* initialise the cache with the base values of the sequence */
static void init_cache(void)
{
    if (cache == NULL) {
        ensure_cache_cap(4);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-2;
        cache[2] = (seq_t)0;
        cache_size = 3;
    }
}

seq_t a(idx_t n)
{
    init_cache();

    if (n >= cache_size) {
        ensure_cache_cap(n + 1);
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t t1 = cache[i - 1];
            seq_t t2 = cache[i - 2];
            seq_t t3 = cache[i - 3];
            seq_t val = -t1 - (seq_t)2 * t2 - (seq_t)2 * t3;
            cache[i] = val;
        }
        cache_size = n + 1;
    }
    return cache[n];
}
