/*
 * A099503  Expansion of 1/(1-4*x+x^3).
 * https://oeis.org/A099503
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - a(n-3), a(0)=1, a(1)=4, a(2)=16
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoised linear‑recurrence computation */
static seq_t *cache = NULL;         /* pointer to computed terms          */
static size_t cache_size = 0;       /* allocated length  (bytes/elem)    */
static size_t cache_len  = 0;       /* number of elements actually filled */

/* Ensure the cache can hold at least n+1 elements */
static void ensure_cache(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (!cache) {
        /* initial allocation; grow to meet need */
        cache_size = need > 4 ? need : 4;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
    } else if (cache_size < need) {
        size_t new_size = cache_size * 2;
        while (new_size < need) new_size *= 2;
        cache = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        cache_size = new_size;
    }
}

/* Initialise the first three values if they are not already set */
static void init_seed_values(void)
{
    if (cache_len == 0) {
        cache[0] = (seq_t)1;
        ++cache_len;
    }
    if (cache_len <= 1) {
        cache[1] = (seq_t)4;
        ++cache_len;
    }
    if (cache_len <= 2) {
        cache[2] = (seq_t)16;
        ++cache_len;
    }
}

seq_t a(idx_t n)
{
    /* Handle trivial indices directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)16;

    ensure_cache(n);
    init_seed_values();

    /* Extend cache up to n, computing by recurrence */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        cache[i] = (seq_t)4 * cache[i-1] - cache[i-3];
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}
