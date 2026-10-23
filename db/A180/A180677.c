/*
 * A180677  The Gi4 sums of the Pell-Jacobsthal triangle A013609.
 * https://oeis.org/A180677
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 9*a(n-1) - 24*a(n-2) + 32*a(n-3) - 16*a(n-4) with a(0)=1,
 *            a(1)=3, a(2)=15, a(3)=87
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, free */

/* Cache for memoisation */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of stored values (indices 0 .. cache_len-1) */
static size_t cache_cap = 0;   /* capacity of the cache */

/* Ensure the cache can hold at least n+1 elements */
static void ensure_cap(idx_t n)
{
    if (cache_len > n)
        return; /* already enough */

    /* Needed capacity */
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return; /* capacity already sufficient */

    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    if (new_cap < needed)
        new_cap = needed;

    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        abort(); /* allocation failure */

    if (cache) {
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }

    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialise the cache with the first four terms */
static void init_cache(void)
{
    if (cache)
        return; /* already initialised */

    cache_cap = 8;
    cache_len = 4;
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache)
        abort();

    cache[0] = (seq_t)1;
    cache[1] = (seq_t)3;
    cache[2] = (seq_t)15;
    cache[3] = (seq_t)87;
}

/* Return the n-th term of the sequence */
seq_t a(idx_t n)
{
    init_cache();
    ensure_cap(n);

    if (n < cache_len)
        return cache[n];

    /* Compute values up to n using the recurrence */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = (seq_t)(9 * cache[i-1]
                          - 24 * cache[i-2]
                          + 32 * cache[i-3]
                          - 16 * cache[i-4]);
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}
