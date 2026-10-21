/*
 * A212752  Number of (w,x,y,z) with all terms in {0,...,n} and at least one
 *          of these conditions holds: w<R, x<R, y<R, z>R, where
 *          R=max{w,x,y,z}-min{w,x,y,z}.
 * https://oeis.org/A212752
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) =
 *            2*a(n-1)+2*a(n-2)-6*a(n-3)+6*a(n-5)-2*a(n-6)-2*a(n-7)+a(n-8);
 *            initial values: a(0)=0, a(1)=14, a(2)=71, a(3)=238, a(4)=580,
 *            a(5)=1224, a(6)=2265, a(7)=3896
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>

/* Static cache to store computed sequence values */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static idx_t computed_max = (idx_t)-1;

/* Ensure the cache has room for indices up to 'needed' (inclusive) */
static void ensure_capacity(idx_t needed)
{
    if ((size_t)needed < cache_size)
        return;
    size_t new_cap = cache_size ? cache_size * 2 : 8;
    if (new_cap <= (size_t)needed)
        new_cap = (size_t)needed + 1;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        return; /* out of memory; caller will fail gracefully */
    if (cache) {
        memcpy(new_cache, cache, cache_size * sizeof(seq_t));
        free(cache);
    }
    cache = new_cache;
    cache_size = new_cap;
}

/* Initialize base values (only once) */
static void init_cache(void)
{
    ensure_capacity(7);
    cache[0] = (seq_t)0;
    cache[1] = (seq_t)14;
    cache[2] = (seq_t)71;
    cache[3] = (seq_t)238;
    cache[4] = (seq_t)580;
    cache[5] = (seq_t)1224;
    cache[6] = (seq_t)2265;
    cache[7] = (seq_t)3896;
    computed_max = 7;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (!cache)
        init_cache();

    if (n <= computed_max)
        return cache[(size_t)n];

    ensure_capacity(n);

    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t val = (seq_t)0;
        /* Positive contributions */
        val += (seq_t)2 * cache[(size_t)(i - 1)];
        val += (seq_t)2 * cache[(size_t)(i - 2)];
        val += (seq_t)6 * cache[(size_t)(i - 5)];
        val += cache[(size_t)(i - 8)];
        /* Negative contributions */
        val -= (seq_t)6 * cache[(size_t)(i - 3)];
        val -= (seq_t)2 * cache[(size_t)(i - 6)];
        val -= (seq_t)2 * cache[(size_t)(i - 7)];
        cache[(size_t)i] = val;
    }
    computed_max = n;
    return cache[(size_t)n];
}
