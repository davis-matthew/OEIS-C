/*
 * A087798  a(n) = 9*a(n-1) + a(n-2), starting with a(0) = 2 and a(1) = 9.
 * https://oeis.org/A087798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9*a(n-1) + a(n-2), a(0)=2, a(1)=9
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements a memoized iterative computation suitable for any seq_t type. */

#include <stdlib.h>
#include <string.h>

static seq_t *cache       = NULL;   /* dynamically increasing cache */
static idx_t  cache_size  = 0;      /* number of valid elements in cache  */
static idx_t  cache_cap   = 0;      /* allocated capacity of cache completely */

/* Ensure the cache can hold at least 'need' elements (indices 0 .. need-1). */
static void ensure_cache_cap(idx_t need)
{
    if (need <= cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap << 1 : 16;
    while (new_cap < need) new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) { /* allocation failed – abort */
        abort();
    }
    if (!cache) {
        /* First allocation – initialise memory to zero for safety. */
        memset(new_ptr, 0, new_cap * sizeof(seq_t));
    }
    cache      = new_ptr;
    cache_cap  = new_cap;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    /* Handle the two base cases without using the cache. */
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)9;

    /* Initialise the cache if necessary */
    if (cache_size == 0) {
        ensure_cache_cap(2);
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)9;
        cache_size = 2;
    }

    /* Return a cached value if possible. */
    if (n < cache_size)
        return cache[n];

    /* Compute new values up to n. */
    ensure_cache_cap(n + 1);
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = (seq_t)9 * cache[i - 1] + cache[i - 2];
    }
    cache_size = n + 1;
    return cache[n];
}
